#ifndef DATAGRID_H
#define DATAGRID_H

#include <godot_cpp/classes/node.hpp>

namespace godot {

class DataGrid : public Node {
	GDCLASS(DataGrid, Node)

private:
	int cell_size; // add size_in_world (exp) and size_in_cells
	Size2i size;
	Point2i center;
	TypedArray<float> data;

protected:
	static void _bind_methods();

public:
	DataGrid();
	~DataGrid();

	int get_cell_size() const { return cell_size; }
	void set_cell_size(int p_cell_size) { cell_size = p_cell_size; }
	const Size2i &get_size() const { return size; }
	void set_size(const Size2i &p_size);
	const Point2i &get_center() const { return center; }
	void set_center(const Point2i &p_position) { return; }
	const Array &get_data() const { return data; }
	void set_data(const TypedArray<float> &p_data) { return; }

	void fill(float p_value) { data.fill(p_value); };

	void add_grid_at_pos(const DataGrid *other_map, Point2i p_position, float magnitude = 1.0f);
	void show_grid();

};

}

#endif // DATAGRID_H