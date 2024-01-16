// Adapted from https://github.com/vllm-project/vllm/blob/v0.2.7/vllm/model_executor/weight_utils.py
// Copyright 2024 The Mako Authors
// Copyright 2023 The vLLM team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "mako/utils/transformers.h"

#include <pwd.h>
#include <unistd.h>

#include <algorithm>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

const auto _default_home                 = fs::path(getpwuid(getuid())->pw_dir) / fs::path(".cache");
const auto default_home                  = _default_home.string();
const auto _xdg_cache_home               = _getenv("XDG_CACHE_HOME", default_home);
const auto __hf_home                     = fs::path(_xdg_cache_home) / fs::path("huggingface");
const std::string _hf_home               = _getenv("HF_HOME", __hf_home.string());
const auto _default_cache_path           = fs::path(_hf_home) / fs::path("hub");
const auto default_cache_path            = _default_cache_path.string();
const std::string _huggingface_hub_cache = _getenv("HUGGINGFACE_HUB_CACHE", default_cache_path);
const std::string _hf_hub_cache          = _getenv("HF_HUB_CACHE", _huggingface_hub_cache);
const std::string _default_revision      = std::string("main");

/// \brief Utility to find weight files from model directory.
/// \param model_name_or_path A path to a directory containing model weights saved using ``save_pretrained``.
/// \param cache_dir Path to the folder where cached files are stored.
/// \param load_format Format of the model to load. Must be one of ``"auto"``, ``"safetensors"``, ``"pt"``, or ``"npcache"``.
/// \param fall_back_to_pt If ``true``, will always allow pt format.
/// \param revision An optional Git revision id which can be a branch name, a tag, or a commit hash.
/// \return List of weight files.
static inline std::tuple<std::string, std::vector<std::string>, bool> prepare_hf_model_weights(
  std::string model_name_or_path,
  std::optional<std::string> cache_dir = std::nullopt,
  std::string load_format              = "auto",
  bool fall_back_to_pt                 = true,
  std::optional<std::string> revision  = std::nullopt) {
  auto is_local        = fs::is_directory(fs::path(model_name_or_path));
  auto use_safetensors = false;

  // Some quantized models use .pt files for storing the weights.
  std::vector<std::string> allow_patterns;
  if (load_format.compare("auto") == 0) {
    allow_patterns = {".safetensors", ".bin"};
  } else if (load_format.compare("safetensors") == 0) {
    use_safetensors = true;
    allow_patterns  = {".safetensors", ".bin"};
  } else if (load_format.compare("pt") == 0) {
    allow_patterns = {".pt"};
  } else if (load_format.compare("npcache") == 0) {
    allow_patterns = {".bin"};
  } else {
    throw std::invalid_argument(std::string("Unknown load format: ").append(load_format));
  }

  if (fall_back_to_pt) {
    allow_patterns.push_back(".pt");
  }

  std::string hf_folder;
  if (!is_local) {
    // Download model weights from Hugging Face Hub.
    // TODO(soomin): implement ``snapshot_download``
  } else {
    hf_folder = model_name_or_path;
  }

  std::vector<std::string> hf_weights_files;
  for (const auto &pattern : allow_patterns) {
    for (const auto &entry : fs::directory_iterator(hf_folder)) {
      if (entry.path().extension().compare(pattern) == 0) {
        hf_weights_files.push_back(entry.path().string());
      }
    }
    if (!hf_weights_files.empty()) {
      if (pattern.compare(".safetensors") == 0) {
        use_safetensors = true;
      }
      break;
    }
  }

  // Exclude files that are not needed for inference.
  // https://github.com/huggingface/transformers/blob/v4.34.0/src/transformers/trainer.py#L227-L233
  if (!use_safetensors) {
    std::vector<std::string> blacklist = {
      "training_args.bin",
      "optimizer.bin",
      "optimizer.pt",
      "scheduler.pt",
      "scaler.pt",
    };
    hf_weights_files.erase(std::remove_if(
      hf_weights_files.begin(),
      hf_weights_files.end(),
      [&](const std::string &entry){
        for (const auto &suffix: blacklist) {
          if (suffix.size() <= entry.size() && entry.compare(entry.size()-suffix.size(), suffix.size(), suffix) == 0) {
            return true;
          }
        }
        return false;
      }), hf_weights_files.end());
  }

  if (hf_weights_files.empty()) {
    throw std::runtime_error(std::string("Cannot find any model weights with ").append(model_name_or_path));
  }

  return std::tuple(hf_folder, hf_weights_files, use_safetensors);
}

std::vector<std::tuple<std::string, torch::Tensor>> mako::utils::hf_model_weights(
  std::string model_name_or_path,
  std::optional<std::string> cache_dir,
  std::string load_format,
  bool fall_back_to_pt,
  std::optional<std::string> revision) {
  auto [hf_folder, hf_weights_files, use_safetensors] = prepare_hf_model_weights(
    model_name_or_path,
    cache_dir,
    load_format,
    fall_back_to_pt,
    revision);

  if (load_format.compare("npcache") == 0) {
    throw std::logic_error(std::string("npcache is currently not supported"));
  }

  if (use_safetensors) {
    throw std::logic_error(std::string("safetensors is currently not supported"));
  }

  std::vector<std::tuple<std::string, torch::Tensor>> weights;

  for (const auto &file : hf_weights_files) {
    auto stream = std::ifstream(file, std::ios::binary);
    auto buf    = std::vector<char>(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    stream.close();
    auto state = torch::pickle_load(buf).toGenericDict();
    for (const auto &weight : state) {
      weights.push_back(std::tuple(weight.key().toStringRef(), weight.value().toTensor()));
    }
  }

  return weights;
}
