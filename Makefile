CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGETS = task1 task2 task3 task4 task5 task6 task7
HEADERS = massiv.h slist.h dlist.h stack.h queue.h tree.h set.h hashtable.h

.PHONY: all clean
all: $(TARGETS)

task1: task1.cpp stack.cpp slist.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) task1.cpp stack.cpp slist.cpp -o $@
task2: task2.cpp set.cpp slist.cpp massiv.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) task2.cpp set.cpp slist.cpp massiv.cpp -o $@
task3: task3.cpp set.cpp slist.cpp massiv.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) task3.cpp set.cpp slist.cpp massiv.cpp -o $@
task4: task4.cpp massiv.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) task4.cpp massiv.cpp -o $@
task5: task5.cpp tree.cpp queue.cpp stack.cpp slist.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) task5.cpp tree.cpp queue.cpp stack.cpp slist.cpp -o $@
task6: task6.cpp hashtable.cpp slist.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) task6.cpp hashtable.cpp slist.cpp -o $@
task7: task7.cpp hashtable.cpp dlist.cpp slist.cpp massiv.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) task7.cpp hashtable.cpp dlist.cpp slist.cpp massiv.cpp -o $@

clean:
	rm -f $(TARGETS)
