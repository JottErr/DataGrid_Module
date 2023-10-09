extends StaticBody2D


@export var layer := 9
@onready var collision_shape: CollisionShape2D = $CollisionShape2D


var imap := InfluenceMap.new()
var cell_size : float


func _ready() -> void:
	cell_size = DataGridHub.get_cell_size()
	var shape := collision_shape.get_shape()
	var rect := shape.get_rect()
	imap.set_cell_size(cell_size)
	imap.set_size(rect.size / cell_size)
	if shape is RectangleShape2D:
		imap.fill(1.0)
	elif shape is CircleShape2D:
		var curve := MathCurve.new()
		curve.set_parameters(0, 0, 0, 1)
		imap.radiate_value_at_position(imap.get_center(), imap.get_center().x, curve)


func register_in_manager(manager: DataGridManager) -> void:
	manager.add_datagrid_centered_to_collection(imap, layer, global_position)

