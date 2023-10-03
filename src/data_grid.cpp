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
	ClassDB::bind_method(D_METHOD("add_grid_centered_at_pos", "other_grid", "position", "magnitude"), &DataGrid::add_grid_centered_at_pos, DEFVAL(1.0f));
	ClassDB::bind_method(D_METHOD("add_from_pos_in_grid", "other_grid", "position", "magnitude"), &DataGrid::add_from_pos_in_grid, DEFVAL(1.0f));
	ClassDB::bind_method(D_METHOD("get_highest_cell"), &DataGrid::get_highest_cell);
	ClassDB::bind_method(D_METHOD("get_lowest_cell"), &DataGrid::get_lowest_cell);
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
			float value = curve->sample_curve(distance);
			data[x + row] = value * magnitude;
		}
	}
}

// Adds the given grid to the current DataGrid. The other grid is centered at the specified position in this grid. 
// The other grid is scaled with the specified magnitude. Used to add a smaller grid into a large grid.
void DataGrid::add_grid_centered_at_pos(const Ref<DataGrid> &other_grid, Point2i p_position, float magnitude) {
	Point2i other_topleft = p_position - other_grid->get_center();
	Point2i other_botright = other_topleft + other_grid->get_size_in_cells();

	Point2i intersection_topleft = other_topleft.max(Point2i(0, 0));
	Point2i intersection_botright = other_botright.min(size_in_cells);

	int other_grid_width = other_grid->get_size_in_cells().x;
	
	for (int y = intersection_topleft.y; y < intersection_botright.y; y++) {
		int row = y * size_in_cells.x;
		int other_row = (y - other_topleft.y) * other_grid_width;
		for (int x = intersection_topleft.x; x < intersection_botright.x; x++) {
			int other_x = x - other_topleft.x;
			float other_value = other_grid->data[other_x + other_row];
			float value = data[x + row];
			data[x + row] = value + other_value * magnitude;
		}
	}
}
// Adds from the given grid to the current DataGrid. A region with the size of the this grid is defined around the specified position in the other grid.
// Values added from the other grid are scaled with the specified magnitude. Used to add part of a large grid into a smaller grid.
void DataGrid::add_from_pos_in_grid(const Ref<DataGrid> &other_grid, Point2i p_position, float magnitude) {
	Point2i topleft_in_other = p_position - center;
	Point2i botright_in_other = topleft_in_other + size_in_cells;

	Point2i intersection_topleft = topleft_in_other.max(Point2i(0, 0));
	Point2i intersection_botright = botright_in_other.min(other_grid->get_size_in_cells());

	int other_grid_width = other_grid->get_size_in_cells().x;

	for (int y = intersection_topleft.y; y < intersection_botright.y; y++) {
		int row = (y - topleft_in_other.y) * size_in_cells.x;
		int other_row = y * other_grid_width;
		for (int x = intersection_topleft.x; x < intersection_botright.x; x++) {
			int this_x = x - topleft_in_other.x;
			float other_value = other_grid->data[x + other_row];
			float value = data[this_x + row];
			data[this_x + row] = value + other_value * magnitude;
		}
	}
}

Point2i DataGrid::get_highest_cell() const {
	float maxValue = -INFINITY;
	Size2i position = Vector2i();

	Vector2i offset;
	offset.x = UtilityFunctions::randi_range(0, size_in_cells.x);
	offset.y = UtilityFunctions::randi_range(0, size_in_cells.y);

	for (int y = 0; y < size_in_cells.y; y++) {
		int rand_y = (y + offset.y) % size_in_cells.y;
		int row = rand_y * size_in_cells.x;
		for (int x = 0; x < size_in_cells.x; x++) {
			int rand_x = (x + offset.x) % size_in_cells.x;
			float value = data[rand_x + row];
			if (value > maxValue) {
				maxValue = value;
				position = Vector2i(rand_x, rand_y);
			}
		}
	}
	return position;
}

Point2i DataGrid::get_lowest_cell() const {
	float minValue = INFINITY;
	Size2i position = Vector2i();

	Vector2i offset;
	offset.x = UtilityFunctions::randi_range(0, size_in_cells.x);
	offset.y = UtilityFunctions::randi_range(0, size_in_cells.y);

	for (int y = 0; y < size_in_cells.y; y++) {
		int rand_y = (y + offset.y) % size_in_cells.y;
		int row = rand_y * size_in_cells.x;
		for (int x = 0; x < size_in_cells.x; x++) {
			int rand_x = (x + offset.x) % size_in_cells.x;
			float value = data[rand_x + row];
			if (value < minValue) {
				minValue = value;
				position = Vector2i(rand_x, rand_y);
			}
		}
	}
	return position;
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