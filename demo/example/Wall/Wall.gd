extends StaticBody2D


@export var layer := 9
@onready var collision_shape: CollisionShape2D = $CollisionShape2D


var datagrid := DataGrid.new()
var cell_size : float


func _ready() -> void:
	cell_size = DataGridHub.get_cell_size()
	var shape := collision_shape.get_shape()
	var rect := shape.get_rect()
	datagrid.set_cell_size(cell_size)
	datagrid.set_size_in_cells(rect.size / cell_size)
	if shape is RectangleShape2D:
		datagrid.fill(1.0)
	elif shape is CircleShape2D:
		var curve := MathCurve.new()
		curve.set_parameters(0, 0, 0, 1)
		datagrid.radiate_value_at_position(datagrid.get_center(), datagrid.get_center().x, curve)


func register_in_manager(manager: DataGridManager) -> void:
	manager.add_datagrid_centered_to_collection(datagrid, layer, global_position)

