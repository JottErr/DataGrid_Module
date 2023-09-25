#ifndef DATAGRIDCOLLECTION_H
#define DATAGRIDCOLLECTION_H

#include <godot_cpp/classes/ref_counted.hpp>
#include "data_grid.h"

namespace godot {

class DataGridCollection : public RefCounted {
	GDCLASS(DataGridCollection, RefCounted)

private:
	Dictionary layered_datagrid_collection;

protected:
	static void _bind_methods();

public:
	DataGridCollection();
	~DataGridCollection();

	void clear() { layered_datagrid_collection.clear(); }
	void add_datagrid_to_collection(const Point2i &p_datagrid_position, int p_layer, const Ref<DataGrid> &p_datagrid);
	bool has_layer_at_pos(const Point2i &p_datagrid_position, int p_layer) const;
	Ref<DataGrid> get_layer_at_pos(const Point2i &p_datagrid_position, int p_layer) const;
	Dictionary filter_layers_at_pos(const Point2i &p_datagrid_coords, const Array &filter_layers) const;
};
}

#endif // DATAGRIDCOLLECTION_H