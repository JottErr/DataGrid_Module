#include "data_grid.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void DataGrid::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_cell_size"), &DataGrid::get_cell_size);
	ClassDB::bind_method(D_METHOD("set_cell_size", "cell_size"), &DataGrid::set_cell_size);
	ClassDB::bind_method(D_METHOD("get_size_in_cells"), &DataGrid::get_size_in_cells);
	ClassDB::bind_method(D_METHOD("set_size_in_cells", "size"), &DataGrid::set_size_in_cells);
	ClassDB::bind_method(D_METHOD("get_center"), &DataGrid::get_center);
	ClassDB::bind_method(D_METHOD("set_center", "center"), &DataGrid::set_center);
	ClassDB::bind_method(D_METHOD("get_data"), &DataGrid::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "data"), &DataGrid::set_data);

	ClassDB::bind_method(D_METHOD("fill", "value"), &DataGrid::fill);
	ClassDB::bind_method(D_METHOD("radiate_value_at_position", "position", "radius", "curve", "magnitude"), &DataGrid::radiate_value_at_position, DEFVAL(1.0f));
	ClassDB::bind_method(D_METHOD("add_grid_at_pos", "other_grid", "position", "magnitude"), &DataGrid::add_grid_at_pos, DEFVAL(1.0f));
	ClassDB::bind_method(D_METHOD("show_grid"), &DataGrid::show_grid);
	
	ClassDB::add_property("DataGrid", PropertyInfo(Variant::INT, "cell_size"), "set_cell_size", "get_cell_size");
	ClassDB::add_property("DataGrid", PropertyInfo(Variant::VECTOR2I, "size_in_cells"), "set_size_in_cells", "get_size_in_cells");
	ClassDB::add_property("DataGrid", PropertyInfo(Variant::VECTOR2I, "center"), "set_center", "get_center");
	ClassDB::add_property("DataGrid", PropertyInfo(Variant::ARRAY, "data"), "set_data", "get_data");
}

DataGrid::DataGrid() {
	cell_size = 1;
	size_in_cells = Size2i(0, 0);
	center = Point2i(0, 0);
}

DataGrid::~DataGrid() {
}

void godot::DataGrid::set_cell_size(int p_cell_size) { 
	cell_size = p_cell_size; 
}

void DataGrid::set_size_in_cells(const Size2i &p_size_in_cells) {
	size_in_cells = p_size_in_cells;
	center = Point2i((size_in_cells.x / 2), (size_in_cells.y / 2));
	reset_data();
}

void DataGrid::radiate_value_at_position(const Point2i &p_position, int radius, const Ref<MathCurve> &curve, float magnitude) {
	Point2i radius_topleft = p_position - Vector2i(radius, radius);
	Point2i radius_botright = p_position + Vector2i(radius + 1, radius + 1);

	radius_topleft = radius_topleft.max(Point2i(0, 0));
	radius_botright = radius_botright.min(size_in_cells);

	int radius_squared = radius * radius;
	
	for (int y = radius_topleft.y; y < radius_botright.y; y++) {
		int row = y * size_in_cells.x;
		int dy = p_position.y - y;
		for (int x = radius_topleft.x; x < radius_botright.x; x++) {
			int dx = p_position.x - x;
			int distance_squared = dx * dx + dy * dy;
			if (distance_squared > radius_squared) {
				continue;
			}
			float distance = Vector2i(dx, dy).length() / radius;
			float value = curve->calculate_value(distance);
			data[x + row] = value * magnitude;
		}
	}
}

void DataGrid::add_grid_at_pos(const Ref<DataGrid> &other_grid, Point2i p_position, float magnitude) {
	Point2i other_topleft = p_position - other_grid->get_center();
	Point2i other_botright = other_topleft + other_grid->get_size_in_cells();

	Point2i intersection_topleft = other_topleft.max(Point2i(0, 0));
	Point2i intersection_botright = other_botright.min(size_in_cells);
	
	for (int y = intersection_topleft.y; y < intersection_botright.y; y++) {
		int other_y = y - other_topleft.y;
		for (int x = intersection_topleft.x; x < intersection_botright.x; x++) {
			int other_x = x - other_topleft.x;
			float other_value = other_grid->data[other_x + other_y * other_grid->get_size_in_cells().x];
			float value = data[x + y * size_in_cells.x];
			data[x + y * size_in_cells.x] = value + other_value * magnitude;
		}
	}
}

void DataGrid::show_grid() {
	String row = String("");
	for (int y = 0; y < size_in_cells.y; y++) {
		for (int x = 0; x < size_in_cells.x; x++) {
			row += UtilityFunctions::str(data[x + y * size_in_cells.x]).pad_decimals(1) + " ";
		}
		row += "\n";
	}
	UtilityFunctions::print(row);
}
