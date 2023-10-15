class_name ManagerVisualizer
extends Node2D


@export var layer: int = 0
@export var imaps: Dictionary = {}
@export var base_color: Color = Color(1.0, 1.0, 1.0, 1.0)


func _ready() -> void:
	top_level = true
	z_index = -1


func _draw():
	if imaps.is_empty():
		return
	for imap_id in imaps:
		var imap: InfluenceMap = imaps[imap_id]
		if imap == null:
			continue
		var size := imap.get_size()
		var cell_size := imap.get_cell_size()
		var offset: Vector2 = imap_id * size * cell_size
		for y in size.y:
			var y_pos := y * cell_size
			for x in size.x:
				var x_pos := x * cell_size
				var color := base_color
				color.a = imap.get_cell_value(Vector2i(x, y))
				draw_rect(Rect2(x_pos + offset.x, y_pos + offset.y, cell_size, cell_size), color)


func _on_data_grid_manager_updated(imap_collection: Dictionary) -> void:
	imaps.clear()
	for imap_id in imap_collection:
		var layerstack: Dictionary = imap_collection.get(imap_id)
		var imap: InfluenceMap = layerstack.get(layer, null)
		imaps[imap_id] = imap 
	queue_redraw()
