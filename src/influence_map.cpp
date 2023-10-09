#include "influence_map.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void InfluenceMap::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_cell_size"), &InfluenceMap::get_cell_size);
	ClassDB::bind_method(D_METHOD("set_cell_size", "cell_size"), &InfluenceMap::set_cell_size);
	ClassDB::bind_method(D_METHOD("get_size"), &InfluenceMap::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "size"), &InfluenceMap::set_size);
	ClassDB::bind_method(D_METHOD("get_center"), &InfluenceMap::get_center);
	ClassDB::bind_method(D_METHOD("set_center", "center"), &InfluenceMap::set_center);
	ClassDB::bind_method(D_METHOD("get_data"), &InfluenceMap::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "data"), &InfluenceMap::set_data);
	ClassDB::bind_method(D_METHOD("reset_data"), &InfluenceMap::reset_data);

	ClassDB::bind_method(D_METHOD("fill", "value"), &InfluenceMap::fill);
	ClassDB::bind_method(D_METHOD("radiate_value_at_position", "position", "radius", "curve", "magnitude"), &InfluenceMap::radiate_value_at_position, DEFVAL(1.0f));
	ClassDB::bind_method(D_METHOD("add_grid_centered_at_pos", "other_grid", "position", "magnitude", "offset"), &InfluenceMap::add_grid_centered_at_pos, DEFVAL(1.0f), DEFVAL(Vector2i()));
	ClassDB::bind_method(D_METHOD("add_from_pos_in_grid", "other_grid", "position", "magnitude"), &InfluenceMap::add_from_pos_in_grid, DEFVAL(1.0f), DEFVAL(Vector2i()));
	ClassDB::bind_method(D_METHOD("show_grid"), &InfluenceMap::show_grid);
	
	ClassDB::bind_method(D_METHOD("get_highest_cell"), &InfluenceMap::get_highest_cell);
	ClassDB::bind_method(D_METHOD("get_lowest_cell"), &InfluenceMap::get_lowest_cell);
	ClassDB::bind_method(D_METHOD("normalize_data"), &InfluenceMap::normalize_data);
	
	ClassDB::add_property("InfluenceMap", PropertyInfo(Variant::INT, "cell_size"), "set_cell_size", "get_cell_size");
	ClassDB::add_property("InfluenceMap", PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");
	ClassDB::add_property("InfluenceMap", PropertyInfo(Variant::VECTOR2I, "center"), "set_center", "get_center");
	ClassDB::add_property("InfluenceMap", PropertyInfo(Variant::ARRAY, "data"), "set_data", "get_data");
}

InfluenceMap::InfluenceMap() {
	cell_size = 1;
	size = Size2i(0, 0);
	center = Point2i(0, 0);
}

InfluenceMap::~InfluenceMap() {
}

void InfluenceMap::set_cell_size(int p_cell_size) { 
	cell_size = p_cell_size; 
}

void InfluenceMap::set_size(const Size2i &p_size) {
	ERR_FAIL_COND(p_size.x < 0 || p_size.y < 0);
	size = p_size;
	center = Point2i((size.x / 2), (size.y / 2));
	reset_data();
}

void InfluenceMap::radiate_value_at_position(const Point2i &p_position, int radius, const Ref<MathCurve> &curve, float magnitude) {
	Point2i radius_topleft = p_position - Vector2i(radius, radius);
	Point2i radius_botright = p_position + Vector2i(radius + 1, radius + 1);

	radius_topleft = radius_topleft.max(Point2i(0, 0));
	radius_botright = radius_botright.min(size);

	int radius_squared = radius * radius;
	
	for (int y = radius_topleft.y; y < radius_botright.y; y++) {
		int row = y * size.x;
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

// Adds the given grid to the current InfluenceMap. The other grid is centered at the specified position in this grid. 
// The other grid is scaled with the specified magnitude. Used to add a smaller grid into a large grid.
void InfluenceMap::add_grid_centered_at_pos(const Ref<InfluenceMap> &other_grid, const Point2i &p_position, float magnitude, const Point2i &p_offset) {
	Point2i other_topleft = p_position - other_grid->get_center() + p_offset;
	Point2i other_botright = other_topleft + other_grid->get_size();

	Point2i intersection_topleft = other_topleft.max(Point2i(0, 0));
	Point2i intersection_botright = other_botright.min(size);

	int other_grid_width = other_grid->get_size().x;
	
	for (int y = intersection_topleft.y; y < intersection_botright.y; y++) {
		int row = y * size.x;
		int other_row = (y - other_topleft.y) * other_grid_width;
		for (int x = intersection_topleft.x; x < intersection_botright.x; x++) {
			int other_x = x - other_topleft.x;
			float other_value = other_grid->data[other_x + other_row];
			float value = data[x + row];
			data[x + row] = value + other_value * magnitude;
		}
	}
}
// Adds from the given grid to the current InfluenceMap. A region with the size of the this grid is defined around the specified position in the other grid.
// Values added from the other grid are scaled with the specified magnitude. Used to add part of a large grid into a smaller grid.
void InfluenceMap::add_from_pos_in_grid(const Ref<InfluenceMap> &other_grid, const Point2i &p_position, float magnitude, const Point2i &p_offset) {
	Point2i topleft_in_other = p_position - center + p_offset;
	Point2i botright_in_other = topleft_in_other + size;

	Point2i intersection_topleft = topleft_in_other.max(Point2i(0, 0));
	Point2i intersection_botright = botright_in_other.min(other_grid->get_size());

	int other_grid_width = other_grid->get_size().x;

	for (int y = intersection_topleft.y; y < intersection_botright.y; y++) {
		int row = (y - topleft_in_other.y) * size.x;
		int other_row = y * other_grid_width;
		for (int x = intersection_topleft.x; x < intersection_botright.x; x++) {
			int this_x = x - topleft_in_other.x;
			float other_value = other_grid->data[x + other_row];
			float value = data[this_x + row];
			data[this_x + row] = value + other_value * magnitude;
		}
	}
}

Point2i InfluenceMap::get_highest_cell() const {
	float maxValue = -INFINITY;
	Size2i position = Vector2i();

	Vector2i offset;
	offset.x = UtilityFunctions::randi_range(0, size.x);
	offset.y = UtilityFunctions::randi_range(0, size.y);

	for (int y = 0; y < size.y; y++) {
		int rand_y = (y + offset.y) % size.y;
		int row = rand_y * size.x;
		for (int x = 0; x < size.x; x++) {
			int rand_x = (x + offset.x) % size.x;
			float value = data[rand_x + row];
			if (value > maxValue) {
				maxValue = value;
				position = Vector2i(rand_x, rand_y);
			}
		}
	}
	return position;
}

Point2i InfluenceMap::get_lowest_cell() const {
	float minValue = INFINITY;
	Size2i position = Vector2i();

	Vector2i offset;
	offset.x = UtilityFunctions::randi_range(0, size.x);
	offset.y = UtilityFunctions::randi_range(0, size.y);

	for (int y = 0; y < size.y; y++) {
		int rand_y = (y + offset.y) % size.y;
		int row = rand_y * size.x;
		for (int x = 0; x < size.x; x++) {
			int rand_x = (x + offset.x) % size.x;
			float value = data[rand_x + row];
			if (value < minValue) {
				minValue = value;
				position = Vector2i(rand_x, rand_y);
			}
		}
	}
	return position;
}

void InfluenceMap::normalize_data() {
	float minValue = INFINITY;
	float maxValue = -INFINITY;
	for (int i = 0; i < size.x * size.y; i++) {
		float value = data[i];
		if (value < minValue) {
			minValue = value;
		}
		if (value > maxValue) {
			maxValue = value;
		}
	}
	float range = maxValue - minValue;
	if (range == 0.0f) {
		reset_data();
		return;
	}
	for (int i = 0; i < size.x * size.y; i++) {
		data[i] = (float(data[i]) - minValue) / range;
	}
}

void InfluenceMap::show_grid() {
	String row = String("");
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			row += UtilityFunctions::str(data[x + y * size.x]).pad_decimals(1) + " ";
		}
		row += "\n";
	}
	UtilityFunctions::print(row);
}