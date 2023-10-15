extends Node2D


@export var dec_lin : MathCurve
@export var dec_4_poly : MathCurve
@export var dec_6_poly : MathCurve


@onready var imap_manager: IMapManager = $IMapManager
@onready var manager_visualizer: Node2D = $ManagerVisualizer
@onready var tile_map: TileMap = $Map/TileMap

enum InfluenceType {PROX = 1, THREAT = 2, INTEREST = 3}


func _ready() -> void:
	DataGridHub.set_world_grid_manager(imap_manager)
	register_tilemap_wall_layer()
	imap_manager.create_templates(InfluenceType.PROX, 1, 21, dec_lin)
	imap_manager.create_templates(InfluenceType.THREAT, 3, 21, dec_4_poly)
	imap_manager.create_templates(InfluenceType.INTEREST, 11, 31, dec_6_poly)
	get_tree().call_group("IntelligenceControllers", "initialize")


func _input(event: InputEvent) -> void:
	if event is InputEventKey:
		if event.pressed and event.keycode in range(48, 58): #0-9
			manager_visualizer.layer = event.keycode - 48 #keycode to number


func register_tilemap_wall_layer() -> void:
	var cell_size := imap_manager.get_cell_size() #10
	var wall_size := tile_map.get_tileset().get_tile_size() #70x70
	
	var imap := InfluenceMap.new()
	imap.set_cell_size(cell_size)
	imap.set_size(wall_size / cell_size)
	imap.reset_data(1.0) #fill with 1.0
	
	for wall_cell_id in tile_map.get_used_cells(0):
		var global_pos := tile_map.to_global(tile_map.map_to_local(wall_cell_id))
		imap_manager.add_imap_centered_to_collection(imap, 9, global_pos)


func test_performance() -> void:
	var testcurve := MathCurve.new()
	var data1 := InfluenceMap.new()
	data1.set_size(Vector2i(9, 9))
	data1.radiate_value_at_position(Vector2i(4, 4), 4, testcurve, 1.0)
	var data2 := InfluenceMap.new()
	data2.set_size(Vector2i(600, 600))
	for n in 10:
		var start = Time.get_ticks_usec()
		for i in 600:
			for j in 600:
				data2.add_map(data1, Vector2i(i, j), 1.0)
		print(Time.get_ticks_usec() - start)


func test_manager_performance() -> void:
	var manager := IMapManager.new()
	manager.set_world_size(Vector2(6000, 6000))
	manager.set_cell_size(10)
	manager.set_imap_count(Vector2i(6, 6))
	
	manager.create_templates(InfluenceType.PROX, 1, 21, dec_lin)

	for n in 10:
		var start = Time.get_ticks_usec()
		for i in 600:
			for j in 600:
				var imap_template := manager.get_template(InfluenceType.PROX, 4)
				manager.add_imap_centered_to_collection(imap_template, 1, Vector2i(i, j), 1.0)
		print(Time.get_ticks_usec() - start)
