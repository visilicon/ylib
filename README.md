skiplist impl

skiplist_main performance evaluation

||dump-time(us)|load-time(us)
---|---|---
|10000|52,156|50,222
|1000000|80,898|339,761
|10000000|642,976|3,520,693
|50000000|3,164,650|19,933,952
|100000000|12,942,288|47,939,869

skiplist_thd multi thread for skiplist level restruct, performance below:

||dump-time(us)|load-time(us)
---|---|---
|10000|20,471|28,022
|1000000|65,807|196,840
|10000000|623,574|1,922,315
|50000000|3,191,959|11,606,632
|100000000|11,891,452|29,854,225

skiplist_opt traversal once for skiplist level restruct, performance below:

||dump-time(us)|load-time(us)
---|---|---
|10000|13,837|**14,515**
|1000000|75,287|**150,434**
|10000000|689,312|**1,842,474**
|50000000|4,919,623|**9,424,774**
|100000000|12,052,898|**18,920,745**

skiplist_optr dump and load optimization base on skip_list_opt, performance below:

||dump-time(us)|load-time(us)
---|---|---
|10000|44,389|15,287
|1000000|**31,326**|138,563
|10000000|**215,302**|**1,435,733**
|50000000|**1,352,561**|**8,410,503**
|100000000|**9,412,041**|**16,796,663**

point count:100000
  dump:44389
  load:15287
point count:1000000
  dump:31326
  load:138563
point count:10000000
  dump:215302
  load:1435733
point count:50000000
  dump:1352561
  load:8410503
point count:100000000
  dump:9412041
  load:16796663



# HISTORY
- 2017.09.03

  add skiplist_opt traversal once for reconstruct level
  
- 2017.09.02

  add skiplist multi thread version for reconstruct level
  
- 2017.09.01

  init impl skiplist_main.cpp
  
  add skiplist serialization
  
  add dump_file & load_file function
