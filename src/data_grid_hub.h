#ifndef DATAGRIDHUB_H
#define DATAGRIDHUB_H

#include "data_grid_component.h"
#include <godot_cpp/classes/object.hpp>

using namespace godot;

class DataGridHub : public Object {
	GDCLASS(DataGridHub, Object);

private:
	static DataGridHub *singleton;
	TypedArray<DataGridComponent> registered_components;

protected:
	static void _bind_methods();

public:
	static DataGridHub *get_singleton();

	DataGridHub();
	~DataGridHub();

	void hello_singleton();
	void add_component(DataGridComponent *p_datagrid_component);
	TypedArray<DataGridComponent> &get_registered_components() { return registered_components; }
	void remove_components(const TypedArray<int> &p_indices);
};

#endif // DATAGRIDHUB_H