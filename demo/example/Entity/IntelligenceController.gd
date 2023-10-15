extends Node2D


signal found_target (target_pos: Vector2)

@export var entity : Entity

@onready var think_cycle_timer: Timer = $ThinkCycleTimer
@onready var target_sprite: Sprite2D = $TargetSprite
@onready var grid_visualizer: Node2D = $GridVisualizer

var manager: DataGridManager
var interest_grid: InfluenceMap


func _ready() -> void:
	think_cycle_timer.start()


func initialize() -> void:
	manager = DataGridHub.get_world_grid_manager()
	if manager == null:
		print("no manager")
		return
	interest_grid = InfluenceMap.new()
	interest_grid.set_cell_size(manager.get_cell_size())
	#var grid_size := entity.speed * 2 + 1
	#interest_grid.set_size(Vector2i(grid_size, grid_size) / manager.get_cell_size())


func _on_think_cycle_timer_timeout() -> void:
	var imap_size := entity.speed * 2 + 1
	interest_grid.set_size(Vector2i(imap_size, imap_size) / manager.get_cell_size())

	#add layer of world datagrid to interest grid, export pairs of <layer, magnitude> in an Array?
	var own_prox_template := manager.get_template(1, entity.speed)
	interest_grid.add_map(own_prox_template, interest_grid.get_center(), 1.4)
	#combine threat of player and proximity of other entities, more weight to player threat
	manager.add_into_datagrid_from_collection(interest_grid, 1, global_position, -2.0)
	manager.add_into_datagrid_from_collection(interest_grid, 2, global_position, -1.0)
	
	#normalize layer mix and multiply by interest template to favor closer cells 
	#move to module, check same size, nesting loops not needed, loop w*h, set value unchecked
	var interest_template := manager.get_template(3, interest_grid.get_center().x)
	interest_grid.normalize_data()
	for y in interest_grid.get_size().y:
		for x in interest_grid.get_size().x:
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
	var target_pos := Vector2(target * interest_grid.get_cell_size())
	target_sprite.global_position = manager.snap_global_postion_to_cell_center(corner_pos + target_pos) 
	grid_visualizer.global_position = corner_pos
	grid_visualizer._on_data_grid_updated(interest_grid)
