﻿скрипт для распаковки game.graph в набор ltx и назад в рабочую версию
версия x.5
модифицирован для работы с графом от игры версии "Зов Припяти"



основан на коде скрипта ggtool.pl взятого из архива "xray_re-tools-src-10sep2008"
автор оригинала: bardak
автор этой штуки: xiani

замечания:
заголовок (header)
vertex_count, edge_count, level_point_count, level_count при запаковке обновляются пересчитыванием реального количества соответствующих данных

связи (edges)
в самом game.graph хранятся только "game_vertex_id" и "distance"
это то куда идёт связь и как далеко то точки назначения,
а то откуда идет связь определяется родительским вертексом
я добавил "parent_level_vertex_id" чтоб вы могли проследить
в дамп я добавил "target_level_vertex_id" и "target_level_id"(так как "level_vertex_id" могут повторяться в разных уровнях) вместо "game_vertex_id"
так как последний является порядковым номером вертекса и он может меняться если ктото изменит последовательность вертексов (например удалив или добавив уровень)

точки уровня (level_points)
вобщем почти те же изменения что и с связями
и снова внимание! они должны идти последовательно одна за другой группами по родительским вертексам

вертексы (vertices)
level_point_offset, level_point_count, edge_offset, edge_count учитываться не будут и обновятся скриптом сами для того чтоб вам не пришлось в калькуляторе пересчитывать всё
еще в дамп добавил gvid.. вобщемто просто так, для информации, при запаковке он не используется


p.s.: в комплекте подправленные stkutils, они нужны для работы, там была добавлена запаковка формата u24

v x.5
добавлен сброс level_ai вшитого в граф в виде бинарных дампов, как есть

v x.4
добавлен ключ "-lp <level_points_read_mode>"
в котором "level_points_read_mode" может принимать значения:
    bin - бинарный режим, читает и записывает level_point-ы без обработки одним цельным куском. если вам не нужно менять level_point-ы то лучше используйте этот ключ так как это намного ускорит работу скрипта (около 95% размера графа уходит в level_point-ы)
    skip - ничего не делает с level_point-ами (используется в основном для дебага)
добавлен вывод межуровневых связей в отдельный файл "*.info.cross_edges.ltx", файл не учитывается при сборке, он только для информации

v x.3
исправлен огромный баг когда связи могли "переходить" к другим вертексам
изменен способ компиляции связей, позволяющий их произвольно хранить в .ltx (не зависимо от родительского вертекса).. раньше они обязаны были храниться группами