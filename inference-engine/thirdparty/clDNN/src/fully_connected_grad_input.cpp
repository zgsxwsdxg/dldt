/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

///////////////////////////////////////////////////////////////////////////////////////////////////
#include "fully_connected_grad_input_inst.h"
#include "primitive_type_base.h"
#include "error_handler.h"
#include "json_object.h"

namespace cldnn
{
primitive_type_id fully_connected_grad_input_type_id()
{
    static primitive_type_base<fully_connected_grad_input> instance;
    return &instance;
}

layout fully_connected_grad_input_inst::calc_output_layout(fully_connected_grad_input_node const& node)
{
    auto desc = node.get_primitive();
    
    auto input_layout = node.input().get_output_layout();
    auto weights_layout = node.weights().get_output_layout();

    return layout(input_layout.data_type, input_layout.format, tensor(input_layout.size.batch[0], weights_layout.size.feature[0], weights_layout.size.spatial[0], weights_layout.size.spatial[1]));
}

std::string fully_connected_grad_input_inst::to_string(fully_connected_grad_input_node const& node)
{
    auto desc       = node.get_primitive();
    auto node_info  = node.desc_to_json();
    auto weights_id = desc->weights;

    std::stringstream primitive_description;

    json_composite fc_info;
    fc_info.add("weights id", weights_id);

    node_info->add("fully connected grad input info", fc_info);
    node_info->dump(primitive_description);

    return primitive_description.str();
}

fully_connected_grad_input_inst::typed_primitive_inst(network_impl& network, fully_connected_grad_input_node const& node)
    :parent(network, node)
{
    auto input_layout = node.input().get_output_layout();
    auto output_layout = node.get_output_layout();

    CLDNN_ERROR_NOT_EQUAL(node.id(), "Input size", input_layout.size.raw.size(), "output size", output_layout.size.raw.size(), "");

}
}
