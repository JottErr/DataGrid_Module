#ifndef INFLUENCEMAP_H
#define INFLUENCEMAP_H

#include "math_curve.h"

namespace godot {

class InfluenceMap : public Resource {
	GDCLASS(InfluenceMap, Resource)

private:
	int cell_size;
	Size2i size;
	Point2i center;
	TypedArray<float> data;
	
protected:
	static void _bind_methods();

public:
	InfluenceMap();
	~InfluenceMap();
	
	int get_cell_size() const { return cell_size; }
	void set_cell_size(int p_cell_size);
	
	const Size2i &get_size() const { return size; }
	void set_size(const Size2i &p_size);
	
	const Point2i &get_center() const { return center; }
	void set_center(const Point2i &p_position) { return; }
	
	const Array &get_data() const { return data; }
	void set_data(const TypedArray<float> &p_data) { return; }
	void reset_data() { data.resize(size.x * size.y); data.fill(0.0); }

	void fill(float p_value) { data.fill(p_value); };

	void radiate_value_at_position(const Point2i &p_position, int radius, const Ref<MathCurve> &curve, float magnitude = 1.0f);
	void add_grid_centered_at_pos(const Ref<InfluenceMap> &other_grid, const Point2i &p_position, float magnitude = 1.0f, const Point2i &p_offset = Vector2i());
	void add_from_pos_in_grid(const Ref<InfluenceMap> &other_grid, const Point2i &p_position, float magnitude = 1.0f, const Point2i &p_offset = Vector2i());
	void show_grid();

	Point2i get_highest_cell() const;
	Point2i get_lowest_cell() const;
	void normalize_data();
};

}

#endif // INFLUENCEMAP_H