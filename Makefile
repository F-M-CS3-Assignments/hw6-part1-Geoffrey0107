all:
	g++ -Wall -g RedBlackTree.cpp RedBlackTreeTests.cpp -o rbt-tests

run: all
	./rbt-tests

clean:
	rm -rf rbt-tests
