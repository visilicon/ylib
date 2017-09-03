all:skiplist_main skiplist_thd skiplist_opt skiplist_optr
	@echo "build ..."

skiplist_main:
	g++ skiplist_main.cpp -o skiplist_main

skiplist_thd:
	g++ skiplist_thd.cpp -o skiplist_thd -lpthread -lpthread -DLEVEL_RATIO=0.25

skiplist_opt:
	g++ skiplist_opt.cpp -o skiplist_opt

skiplist_optr:
	g++ skiplist_optr.cpp -o skiplist_optr

clean:
	@rm -rf skiplist_main skiplist_thd skiplist_opt skiplist_optr
