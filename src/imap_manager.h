#ifndef IMAPMANAGER_H
#define IMAPMANAGER_H

#include <godot_cpp/classes/node.hpp>
#include "math_curve.h"
#include "influence_map.h"
#include "data_grid_component.h"
#include "godot_cpp/templates/local_vector.hpp"
#include "godot_cpp/templates/hash_map.hpp"

namespace godot {

class IMapManager : public Node {
	GDCLASS(IMapManager, Node)

private:
	Vector2i world_size;
	Vector2i imap_count;
	float cell_size;
	Vector2i imap_size;

	Dictionary imap_collection;

	struct InfluenceMapTemplate {
		int radius;
		Ref<InfluenceMap> imap;
		
		InfluenceMapTemplate() {}
		InfluenceMapTemplate(int p_radius, Ref<InfluenceMap> p_imap) {
			radius = p_radius;
			imap = p_imap;
		}
	};

	Ref<InfluenceMap> out_of_boundaries_template;
	HashMap<int, LocalVector<InfluenceMapTemplate>> hashed_templates;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	IMapManager();
	~IMapManager();

	void set_world_size(const Vector2i &p_world_size);
	const Vector2i &get_world_size() const { return world_size; }
	void set_imap_count(const Vector2i &p_imap_count);
	const Vector2i &get_imap_count() const { return imap_count; }
	void set_cell_size(float p_cell_size);
	float get_cell_size() const { return cell_size; }
	void set_imap_size(const Vector2i &p_imap_size = Vector2i(0, 0));
	const Vector2i &get_imap_size() const { return imap_size; }
	
	void create_templates(int p_type, int min_radius, int max_radius, const Ref<MathCurve> &p_curve);
	Ref<InfluenceMap> get_template(int p_type, int p_radius) const;

	void _process(float p_delta);
	void emit_updated(const Dictionary &imap_collection);

	void add_imap_layer_to_collection(const Vector2i &p_imap_position, int p_layer, const Ref<InfluenceMap> &p_imap);
	bool has_imap_layer(const Vector2i &p_imap_position, int p_layer) const;
	Ref<InfluenceMap> get_imap_layer(const Vector2i &p_imap_position, int p_layer) const;
	Dictionary filter_imap_layers(const Vector2i &p_imap_position, const Array &filter_layers) const;

	Vector2i global_position_to_imap_id(const Vector2i &p_global_position) const;
	bool imap_id_in_bounds(const Vector2i &p_imap_id) const;
	Vector2i global_position_to_imap_cell_id(const Vector2 &p_world_position, const Vector2i &p_imap_id) const;
	PackedVector2Array get_touched_imaps(const Vector2i &p_center_cell, int p_radius) const;
	void add_imap_centered_to_collection(const Ref<InfluenceMap> &grid_to_add, int p_layer, const Point2 &p_global_position, float p_magnitude = 1.0f, bool registering = true);
	void add_into_imap_from_collection(const Ref<InfluenceMap> &grid_to_add_into, int p_layer, const Point2 &p_global_position, float p_magnitude = 1.0f);
	void mark_cells_outside_boundaries(const Ref<InfluenceMap> &p_imap, Vector2i p_global_position, float p_magnitude = 1.0f);
	
	Vector2 snap_global_postion_to_cell_center(const Vector2 &p_global_position) const;
	Vector2 find_imap_corner_from_center(const Vector2 &p_global_position, Vector2i p_imap_center);
};
}

#endif // IMAPMANAGER_H