#ifndef DATAGRID_H
#define DATAGRID_H

#include "math_curve.h"

namespace godot {

class DataGrid : public Resource {
	GDCLASS(DataGrid, Resource)

private:
	int cell_size;
	Size2i size_in_cells;
	Point2i center;
	TypedArray<float> data;
	
protected:
	static void _bind_methods();

public:
	DataGrid();
	~DataGrid();
	
	int get_cell_size() const { return cell_size; }
	void set_cell_size(int p_cell_size);
	
	const Size2i &get_size_in_cells() const { return size_in_cells; }
	void set_size_in_cells(const Size2i &p_size_in_cells);
	
	const Point2i &get_center() const { return center; }
	void set_center(const Point2i &p_position) { return; }
	
	const Array &get_data() const { return data; }
	void set_data(const TypedArray<float> &p_data) { return; }
	void reset_data() { data.resize(size_in_cells.x * size_in_cells.y); data.fill(0.0); }

	void fill(float p_value) { data.fill(p_value); };

	void radiate_value_at_position(const Point2i &p_position, int radius, const MathCurve *curve, float magnitude = 1.0f);
	void add_grid_at_pos(const DataGrid *other_map, Point2i p_position, float magnitude = 1.0f);
	void show_grid();

};

}

#endif // DATAGRID_H