Script for unpacking game.graph to ltx set and back into working version
version x.5
Modified for work with graph from game "Call of Pripyat"

Based on script code of ggtool.pl from archive "xray_re-tools-src-10sep2008"

original author: bardak
autor of this stuff: Xiani

Notifications:
Header
vertex_count, edge_count, level_point_count, level_count during packing upgrades by recalculating of real number corresponded data

Edges
into game.graph itself stores only "game_vertex_id" and "distance"
its a way where connection is going and how far to point of destination,
and from where connection detects by parent vertex
i added "parent_level_vertex_id" so you can follow this
into dump i added "target_level_vertex_id" and "target_level_id"(because "level_vertex_id" can repeat on different levels) instead "game_vertex_id"
because last one is index number of vertex and it can be changed if somebody with change sequence of vertices (for example by deleting or adding new level)

Level points (level_points)
almost exactly same changes as with connection points
and again attention! they need to go sequentially one to another group to parent vertices.

vertices
level_point_offset, level_point_count, edge_offset, edge_count won't be considerated and will be updated by script theirselves so you won't need to recalculate
in calculator all of it. also to dump added gvid.. just for fun if to say true, for information, it's not used in packing


p.s.:in pack also fixed stkutils, they neede for work, in them was setted packing of u24 format


v x.5
added dumping of level_ai inside graph like binary dump, as it is


v x.4
added key "-lp <level_points_read_mode>"
in which "level_points_read_mode" can have values:
    bin - binary view, reads and writes level_points without processing as one whole piece. if you don't need to change  level_points better use this key because it will speed up work of script (near 95% of graph size goes to level_points)
    skip - do nothing with level_points (used mostly for debug)
added output of translevels connections to separate file "*.info.cross_edges.ltx", file not considered when building, its only for information

v x.3
fixed big bug when connection points could "move" to other verticies
changed way of connection points compilation, which allow randomly store them in .ltx (without depend from parent vertex).. earilier they were needed to store in groups