#include "data_grid.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void DataGrid::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_cell_size"), &DataGrid::get_cell_size);
	ClassDB::bind_method(D_METHOD("set_cell_size", "cell_size"), &DataGrid::set_cell_size);
	ClassDB::bind_method(D_METHOD("get_size"), &DataGrid::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "size"), &DataGrid::set_size);
	ClassDB::bind_method(D_METHOD("get_center"), &DataGrid::get_center);
	ClassDB::bind_method(D_METHOD("set_center", "center"), &DataGrid::set_center);
	ClassDB::bind_method(D_METHOD("get_data"), &DataGrid::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "data"), &DataGrid::set_data);

	ClassDB::bind_method(D_METHOD("fill", "value"), &DataGrid::fill);
	ClassDB::bind_method(D_METHOD("add_grid_at_pos", "other_grid", "p_position", "magnitude"), &DataGrid::add_grid_at_pos);
	ClassDB::bind_method(D_METHOD("show_grid"), &DataGrid::show_grid);

	ClassDB::add_property("DataGrid", PropertyInfo(Variant::INT, "cell_size"), "set_cell_size", "get_cell_size");
	ClassDB::add_property("DataGrid", PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");
	ClassDB::add_property("DataGrid", PropertyInfo(Variant::VECTOR2I, "center"), "set_center", "get_center");
	ClassDB::add_property("DataGrid", PropertyInfo(Variant::ARRAY, "data"), "set_data", "get_data");
}

DataGrid::DataGrid() {
	cell_size = 1;
	size = Size2i(0, 0);
	center = Point2i(0, 0);
}

DataGrid::~DataGrid() {
}

void DataGrid::set_size(const Size2i &p_size) {
	size = p_size;
	center = Point2i((p_size.x / 2), (p_size.y / 2));
	data.resize(p_size.x * p_size.y);
	data.fill(0.0);
}

void DataGrid::add_grid_at_pos(const DataGrid *other_grid, Vector2i p_position, float magnitude) {
	Point2i other_topleft = p_position - other_grid->get_center();
	Point2i other_botright = other_topleft + other_grid->get_size();

	Point2i intersection_topleft = other_topleft.max(Point2i(0, 0));
	Point2i intersection_botright = other_botright.min(size);
	
	for (int y = intersection_topleft.y; y < intersection_botright.y; y++) {
		for (int x = intersection_topleft.x; x < intersection_botright.x; x++) {
			int other_x = x - intersection_topleft.x;
			int other_y = y - intersection_topleft.y;
			float other_value = other_grid->data[other_x + other_y * other_grid->size.x];
			float value = data[x + y * size.x];
			data[x + y * size.x] = value + other_value * magnitude;
		}
	}
}

void DataGrid::show_grid() {
	for (int y = 0; y < size.y; y++) {
		Array row;
		row.resize(size.x);
		for (int x = 0; x < size.x; x++) {
			row[x] = data[x + y * size.x];
		}
		UtilityFunctions::print(row);
	}
}