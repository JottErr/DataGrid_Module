#ifndef DATAGRIDMANAGER_H
#define DATAGRIDMANAGER_H

#include <godot_cpp/classes/node.hpp>
#include "data_grid.h"

namespace godot {

class DataGridManager : public Node {
	GDCLASS(DataGridManager, Node)

private:
	Size2i world_size; // Size of the covered area in world units
	Size2i datagrid_count; // Number of DataGrids across the covered area
	int cell_size; // Size of a cell in world units
	Size2i datagrid_size; // Size of each DataGrid in cells
	
	float update_frequency;

	Ref<DataGrid> datagrid;

protected:
	static void _bind_methods();

public:
	DataGridManager();
	~DataGridManager();

	const Size2i &get_world_size() const { return world_size; }
	void set_world_size(const Size2i &p_world_size);

	const Size2i &get_datagrid_count() const { return datagrid_count; }
	void set_datagrid_count(const Size2i &p_datagrid_count) { return; }
	
	int get_cell_size() const { return cell_size; }
	void set_cell_size(int p_cell_size);

	const Size2i &get_datagrid_size() const { return datagrid_size; }
	void set_datagrid_size(const Size2i &p_datagrid_size) { return; }

	float get_update_frequency() const { return update_frequency; }
	void set_update_frequency(float p_update_frequency) { update_frequency = p_update_frequency; }

	Ref<DataGrid> get_datagrid() const { return datagrid; }
};
}

#endif // DATAGRIDMANAGER_H