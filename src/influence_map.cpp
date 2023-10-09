#include "influence_map.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void InfluenceMap::set_cell_size(int p_cell_size) { 
	cell_size = p_cell_size; 
}

void InfluenceMap::set_size(const Vector2i &p_size) {
	ERR_FAIL_COND(p_size.x < 0 || p_size.y < 0);
	size = p_size;
	center = Vector2i((size.x / 2), (size.y / 2));
	reset_data();
}

void InfluenceMap::reset_data(float p_value) {
	data.clear();
	for (int i = 0; i < size.x * size.y; i++) {
		data.push_back(p_value);
	}
}

bool InfluenceMap::is_in_bounds(const Vector2i &p_id) const {
	Vector2i position = Vector2i(0, 0); //maybe class member?
	if (p_id.x < position.x) {
		return false;
	}
	if (p_id.y < position.y) {
		return false;
	}

	if (p_id.x >= (position.x + size.x)) {
		return false;
	}
	if (p_id.y >= (position.y + size.y)) {
		return false;
	}

	return true;
}

void InfluenceMap::set_cell_value(const Vector2i &p_id, float p_value) {
	ERR_FAIL_COND_MSG(!is_in_bounds(p_id), vformat("Can't set if point is disabled. Point %s out of bounds %s - %s.", p_id, "(0, 0)", size));
	data[p_id.x + p_id.y * size.x] = p_value;
}

float InfluenceMap::get_cell_value(const Vector2i &p_id) const {
	ERR_FAIL_COND_V_MSG(!is_in_bounds(p_id), false, vformat("Can't get if point is disabled. Point %s out of bounds %s - %s.", p_id, "(0, 0)", size));
	return data[p_id.x + p_id.y * size.x];
}

void InfluenceMap::radiate_value_at_position(const Vector2i &p_position, int radius, const Ref<MathCurve> &curve, float magnitude) {
	Vector2i radius_topleft = p_position - Vector2i(radius, radius);
	Vector2i radius_botright = p_position + Vector2i(radius + 1, radius + 1);

	radius_topleft = radius_topleft.max(Vector2i(0, 0));
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
void InfluenceMap::add_map(const Ref<InfluenceMap> &other_map, const Vector2i &p_position, float magnitude, const Vector2i &p_offset) {
	Vector2i other_topleft = p_position - other_map->get_center() + p_offset;
	Vector2i other_botright = other_topleft + other_map->get_size();

	Vector2i intersection_topleft = other_topleft.max(Vector2i(0, 0));
	Vector2i intersection_botright = other_botright.min(size);

	int other_map_width = other_map->get_size().x;
	
	for (int y = intersection_topleft.y; y < intersection_botright.y; y++) {
		int row = y * size.x;
		int other_row = (y - other_topleft.y) * other_map_width;
		for (int x = intersection_topleft.x; x < intersection_botright.x; x++) {
			int other_x = x - other_topleft.x;
			data[x + row] += other_map->data[other_x + other_row] * magnitude;
		}
	}
}
// Adds from the given grid to the current InfluenceMap. A region with the size of the this grid is defined around the specified position in the other grid.
// Values added from the other grid are scaled with the specified magnitude. Used to add part of a large grid into a smaller grid.
void InfluenceMap::add_from_map(const Ref<InfluenceMap> &other_map, const Vector2i &p_position, float magnitude, const Vector2i &p_offset) {
	Vector2i topleft_in_other = p_position - center + p_offset;
	Vector2i botright_in_other = topleft_in_other + size;

	Vector2i intersection_topleft = topleft_in_other.max(Vector2i(0, 0));
	Vector2i intersection_botright = botright_in_other.min(other_map->get_size());

	int other_map_width = other_map->get_size().x;

	for (int y = intersection_topleft.y; y < intersection_botright.y; y++) {
		int row = (y - topleft_in_other.y) * size.x;
		int other_row = y * other_map_width;
		for (int x = intersection_topleft.x; x < intersection_botright.x; x++) {
			int this_x = x - topleft_in_other.x;
			data[this_x + row] += other_map->data[x + other_row] * magnitude;
		}
	}
}

Vector2i InfluenceMap::get_highest_cell() const {
	float maxValue = -INFINITY;
	Vector2i position = Vector2i();

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

Vector2i InfluenceMap::get_lowest_cell() const {
	float minValue = INFINITY;
	Vector2i position = Vector2i();

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
		data[i] = (data[i] - minValue) / range;
	}
}

void InfluenceMap::show_map() {
	String row = String("");
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			row += UtilityFunctions::str(data[x + y * size.x]).pad_decimals(1) + " ";
		}
		row += "\n";
	}
	UtilityFunctions::print(row);
}

void InfluenceMap::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_cell_size", "cell_size"), &InfluenceMap::set_cell_size);
	ClassDB::bind_method(D_METHOD("get_cell_size"), &InfluenceMap::get_cell_size);
	ClassDB::bind_method(D_METHOD("set_size", "size"), &InfluenceMap::set_size);
	ClassDB::bind_method(D_METHOD("get_size"), &InfluenceMap::get_size);
	ClassDB::bind_method(D_METHOD("set_center", "center"), &InfluenceMap::set_center);
	ClassDB::bind_method(D_METHOD("get_center"), &InfluenceMap::get_center);
	ClassDB::bind_method(D_METHOD("reset_data", "initial_value"), &InfluenceMap::reset_data, DEFVAL(0.0f));
	ClassDB::bind_method(D_METHOD("is_in_bounds", "id"), &InfluenceMap::is_in_bounds);
	ClassDB::bind_method(D_METHOD("set_cell_value", "id", "value"), &InfluenceMap::set_cell_value);
	ClassDB::bind_method(D_METHOD("get_cell_value", "id"), &InfluenceMap::get_cell_value);
	
	ClassDB::bind_method(D_METHOD("radiate_value_at_position", "position", "radius", "curve", "magnitude"), &InfluenceMap::radiate_value_at_position, DEFVAL(1.0f));
	ClassDB::bind_method(D_METHOD("add_map", "other_map", "position", "magnitude", "offset"), &InfluenceMap::add_map, DEFVAL(1.0f), DEFVAL(Vector2i()));
	ClassDB::bind_method(D_METHOD("add_from_map", "other_map", "position", "magnitude", "offset"), &InfluenceMap::add_from_map, DEFVAL(1.0f), DEFVAL(Vector2i()));
	
	ClassDB::bind_method(D_METHOD("get_highest_cell"), &InfluenceMap::get_highest_cell);
	ClassDB::bind_method(D_METHOD("get_lowest_cell"), &InfluenceMap::get_lowest_cell);
	ClassDB::bind_method(D_METHOD("normalize_data"), &InfluenceMap::normalize_data);

	ClassDB::bind_method(D_METHOD("show_grid"), &InfluenceMap::show_map);
	
	ClassDB::add_property("InfluenceMap", PropertyInfo(Variant::INT, "cell_size"), "set_cell_size", "get_cell_size");
	ClassDB::add_property("InfluenceMap", PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");
	ClassDB::add_property("InfluenceMap", PropertyInfo(Variant::VECTOR2I, "center"), "set_center", "get_center");
}

InfluenceMap::InfluenceMap() {
	cell_size = 1;
	size = Vector2i(0, 0);
	center = Vector2i(0, 0);
}

InfluenceMap::~InfluenceMap() {
}