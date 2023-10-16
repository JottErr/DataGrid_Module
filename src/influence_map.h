#ifndef INFLUENCEMAP_H
#define INFLUENCEMAP_H

#include "math_curve.h"

namespace godot {

class InfluenceMap : public Resource {
	GDCLASS(InfluenceMap, Resource)

private:
	float cell_size;
	Vector2i size;
	Vector2i center;
	PackedFloat32Array data;
	
protected:
	static void _bind_methods();

public:
	InfluenceMap();
	~InfluenceMap();
	
	void set_cell_size(float p_cell_size);
	float get_cell_size() const { return cell_size; }
	void set_size(const Vector2i &p_size);
	const Vector2i &get_size() const { return size; }
	void set_center(const Vector2i &p_position) { return; }
	const Vector2i &get_center() const { return center; }
	void reset_data(float p_value = 0.0f);
	bool is_in_bounds(const Vector2i &p_id) const;
	void set_cell_value(const Vector2i &p_id, float p_value);
	float get_cell_value(const Vector2i &p_id) const;
	
	void radiate_value_at_position(const Vector2i &p_position, int radius, const Ref<MathCurve> &curve, float magnitude = 1.0f);
	void add_map(const Ref<InfluenceMap> &other_map, const Vector2i &p_position, float magnitude = 1.0f, const Vector2i &p_offset = Vector2i());
	void add_from_map(const Ref<InfluenceMap> &other_map, const Vector2i &p_position, float magnitude = 1.0f, const Vector2i &p_offset = Vector2i());

	Vector2i get_highest_cell() const;
	Vector2i get_lowest_cell() const;
	void normalize_data();

	void show_map();
};

}

#endif // INFLUENCEMAP_H