@tool
extends Node2D


@export var manager: IMapManager : set = set_manager


func _draw():
	var imap_count := manager.get_imap_count()
	var imap_size := manager.get_imap_size()
	var cell_size := manager.get_cell_size()
	
	for y in imap_count.y:
		for x in imap_count.x:
			var start := Vector2(0.5, 0.5) + Vector2(x, y) * Vector2(imap_size) * cell_size
			draw_rect(Rect2(start, imap_size * cell_size), Color.BLACK, false, 1.0)


func set_manager(_manager: IMapManager) -> void:
	manager = _manager
	queue_redraw()
