class_name GridVisualizer
extends Node2D


@export var imap: InfluenceMap = null
@export var base_color: Color = Color(1.0, 1.0, 1.0, 1.0)


func _ready() -> void:
	top_level = true
	z_index = -1


func _draw():
	if imap == null:
		return
	var data := imap.get_data()
	var size := imap.get_size()
	var cell_size := imap.get_cell_size()
	for y in size.y:
		var row := y * size.x
		var y_pos := y * cell_size
		for x in size.x:
			var x_pos := x * cell_size
			var color := base_color
			color.a = data[x + row]
			draw_rect(Rect2(x_pos, y_pos, cell_size, cell_size), color)


func _on_data_grid_updated(_datagrid: InfluenceMap) -> void:
	imap = _datagrid
	queue_redraw()
