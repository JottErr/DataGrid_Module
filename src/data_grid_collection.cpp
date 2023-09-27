#include "data_grid_collection.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void DataGridCollection::_bind_methods() {
	ClassDB::bind_method(D_METHOD("clear"), &DataGridCollection::clear);
	ClassDB::bind_method(D_METHOD("add_datagrid_to_collection", "datagrid_position", "layer", "datagrid"), &DataGridCollection::add_datagrid_to_collection);
	ClassDB::bind_method(D_METHOD("has_layer_at_pos", "datagrid_position", "layer"), &DataGridCollection::has_layer_at_pos);
	ClassDB::bind_method(D_METHOD("get_layer_at_pos", "datagrid_position", "layer"), &DataGridCollection::get_layer_at_pos);
	ClassDB::bind_method(D_METHOD("filter_layers_at_pos", "datagrid_position", "filter_layers"), &DataGridCollection::filter_layers_at_pos);
}

DataGridCollection::DataGridCollection() {
}

DataGridCollection::~DataGridCollection() {
}

void DataGridCollection::add_datagrid_to_collection(const Point2i &p_datagrid_position, int p_layer, const Ref<DataGrid> &p_datagrid) {
	if (!layered_datagrid_collection.has(p_datagrid_position)) {
		Dictionary layer_stack;
		layer_stack[p_layer] = p_datagrid;
		layered_datagrid_collection[p_datagrid_position] = layer_stack;
		return;
	}
	Dictionary layer_stack = layered_datagrid_collection[p_datagrid_position];
	if (!layer_stack.has(p_layer)) {
		layer_stack[p_layer] = p_datagrid;
	}
}

Dictionary DataGridCollection::filter_layers_at_pos(const Point2i &p_datagrid_position, const Array &filter_layers) const {
	Dictionary result; // return an TypedArray[DataGrid].resize[max_layers?20], fill with null
	if (layered_datagrid_collection.has(p_datagrid_position)) {
		Dictionary all_layers = layered_datagrid_collection[p_datagrid_position];
		if (filter_layers.is_empty()) {
			return all_layers;
		}
		for (int i = 0; i < filter_layers.size(); i++) {
			int layer = filter_layers[i];
			if (all_layers.has(layer)) {
				result[layer] = all_layers[layer];
			}
		}
	}
	return result;
}

bool DataGridCollection::has_layer_at_pos(const Point2i &p_datagrid_position, int p_layer) const {
	if (!layered_datagrid_collection.has(p_datagrid_position)) {
		return false;
	}
	Dictionary layer_stack = layered_datagrid_collection[p_datagrid_position];
	return layer_stack.has(p_layer);
}

Ref<DataGrid> DataGridCollection::get_layer_at_pos(const Point2i &p_datagrid_position, int p_layer) const {
	if (!this->has_layer_at_pos(p_datagrid_position, p_layer)) {
		return nullptr;
	}
	Dictionary layer_stack = layered_datagrid_collection[p_datagrid_position];
	Ref<DataGrid> layer = layer_stack[p_layer];
	return layer;
}
