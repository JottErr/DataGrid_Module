#ifndef DATAGRIDHUB_H
#define DATAGRIDHUB_H

#include "data_grid_component.h"
#include "imap_manager.h"
#include <godot_cpp/classes/object.hpp>

using namespace godot;

class DataGridHub : public Object {
	GDCLASS(DataGridHub, Object);

private:
	static DataGridHub *singleton;
	IMapManager *world_grid_manager;
	TypedArray<DataGridCompRef> registered_components;

protected:
	static void _bind_methods();

public:
	static DataGridHub *get_singleton();
	const float cell_size = 10.0f;

	DataGridHub();
	~DataGridHub();

	float get_cell_size() const { return cell_size; };
	void set_world_grid_manager(IMapManager *p_world_grid_manager);
	IMapManager *get_world_grid_manager() const;
	void add_component_data_res(Ref<DataGridCompRef> p_datagrid_component_data_res);
	TypedArray<DataGridCompRef> get_registered_components_data_res() { return registered_components; }
	void remove_components(const TypedArray<int> &p_indices);
};

#endif // DATAGRIDHUB_H