extends Node2D

signal found_target (target_pos: Vector2)

@export var interest_grid : InfluenceMap
@export var interest_grid_size := Vector2i(31, 31)

@onready var think_cycle_timer: Timer = $ThinkCycleTimer
@onready var target_sprite: Sprite2D = $TargetSprite
@onready var grid_visualizer: Node2D = $GridVisualizer

var cell_size := 10.0
var cell_center := Vector2(cell_size, cell_size) / 2.0

var interest_template := InfluenceMap.new()


func _ready() -> void:
	interest_grid = InfluenceMap.new()
	interest_grid.set_cell_size(cell_size)
	interest_grid.set_size(interest_grid_size)
	create_interest_template()
	think_cycle_timer.start()


func _on_think_cycle_timer_timeout() -> void:
	interest_grid.reset_data()

	#add layer of world datagrid to interest grid, export pairs of <layer, magnitude> in an Array?
	var manager: DataGridManager = DataGridHub.get_world_grid_manager()
	if manager == null:
		print("no manager")
		return
	var lin_curve := MathCurve.new()
	lin_curve.set_parameters(-1, 1, 0, 1)
	interest_grid.radiate_value_at_position(interest_grid.get_center(), 7, lin_curve, 1.0) #own pos
	#combine threat of player and proximity of other entities, more weight to player threat
	manager.add_into_datagrid_from_collection(interest_grid, 1, global_position, -2.0)
	manager.add_into_datagrid_from_collection(interest_grid, 2, global_position, -1.0)
	
	
	
	#normalize layer mix and multiply by interest template to favor closer cells
	interest_grid.normalize_data()
	for y in interest_grid_size.y:
		for x in interest_grid_size.x:
			var value := interest_grid.get_cell_value(Vector2i(x, y)) * interest_template.get_cell_value(Vector2i(x, y))
			interest_grid.set_cell_value(Vector2i(x, y), value)
	
	#set blocked cells (walls) to 0
	manager.add_into_datagrid_from_collection(interest_grid, 9, global_position, -2.0)
	#set cells out of boundaries to 0
	manager.mark_cells_outside_boundaries(interest_grid, global_position, -2.0)


	#Vector in cells from Entity to target, used to get a direction atm, could be made global_position
	var target: Vector2i = interest_grid.get_highest_cell()
	found_target.emit(Vector2(target - interest_grid.get_center()))


	var corner_pos := manager.find_corner_from_center(global_position, interest_grid.get_center())
	target_sprite.global_position = manager.snap_global_postion_to_cell_center(corner_pos + target * cell_size) 
	grid_visualizer.global_position = corner_pos
	grid_visualizer._on_data_grid_updated(interest_grid)


func create_interest_template() -> void:
	var curve := MathCurve.new()
	curve.set_parameters(-1.0, 4, 0.0, 1.0)
	interest_template.set_cell_size(cell_size)
	interest_template.set_size(interest_grid_size)
	interest_template.radiate_value_at_position(interest_template.get_center(), interest_template.get_center().x, curve)
