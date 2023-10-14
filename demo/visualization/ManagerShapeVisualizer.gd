@tool
extends Node2D


@export var manager : DataGridManager : set = set_manager


func _draw():
	var datagrid_count := manager.get_datagrid_count()
	var datagrid_size := manager.get_datagrid_size()
	var cell_size := manager.get_cell_size()
	
	for y in datagrid_count.y:
		for x in datagrid_count.x:
			var start := Vector2(0.5, 0.5) + Vector2(x, y) * Vector2(datagrid_size) * cell_size
			draw_rect(Rect2(start, datagrid_size * cell_size), Color.BLACK, false, 1.0)


func set_manager(_manager: DataGridManager) -> void:
	manager = _manager
	queue_redraw()
