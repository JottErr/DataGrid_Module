@tool
extends Node2D


@export var datagrid_count: Vector2i : set = set_datagrid_count
@export var datagrid_size: Vector2i : set = set_datagrid_size
@export var cell_size: int : set = set_cell_size


func _draw():
	for y in datagrid_count.y:
		for x in datagrid_count.x:
			var start := Vector2(0.5, 0.5) + Vector2(x, y) * Vector2(datagrid_size) * cell_size
			draw_rect(Rect2(start, datagrid_size * cell_size), Color.BLACK, false, 1.0)


func set_datagrid_count(value: Vector2i) -> void:
	datagrid_count = value
	queue_redraw()


func set_datagrid_size(value: Vector2i) -> void:
	datagrid_size = value
	queue_redraw()


func set_cell_size(value: int) -> void:
	cell_size = value
	queue_redraw()
