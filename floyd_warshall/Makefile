include ../tbbsuite/Makefile.include

INCLUDE_DIR = $(TBB_DIR)/include/
LIBRARY_DIR = $(TBB_DIR)/build/$(LIBRARY_VER)

CXX = g++
CXXFLAGS = -O3 -Wall -std=c++0x #-DTESTCORRECT 
LDFLAGS =
LIBS = -ltbb -lrt 

CXXFLAGS += -I$(INCLUDE_DIR)
 
all : fw_standard fw_rec fw_tiled 

fw_standard : fw_standard.o fw_standard_driver.o fw_util.o 
	$(CXX) $(LDFLAGS) fw_standard_driver.o fw_standard.o fw_util.o -o fw_standard -L$(LIBRARY_DIR) $(LIBS)

fw_rec : fw_rec.o fw_rec_driver.o fw_util.o 
	$(CXX) $(LDFLAGS) fw_rec.o fw_rec_driver.o fw_util.o -o fw_rec -L$(LIBRARY_DIR) $(LIBS)

fw_tiled : fw_tiled.o fw_tiled_driver.o fw_util.o 
	$(CXX) $(LDFLAGS) fw_tiled.o fw_tiled_driver.o fw_util.o -o fw_tiled -L$(LIBRARY_DIR) $(LIBS)

fw_tiled_tasks : fw_tiled_tasks_raw.o fw_util.o 
	$(CXX) $(LDFLAGS) fw_tiled_tasks_raw.o fw_util.o -o fw_tiled_tasks -L$(LIBRARY_DIR) $(LIBS)

fw_tiled_tasks_recycle : fw_tiled_tasks_raw_recycle.o fw_util.o 
	$(CXX) $(LDFLAGS) fw_tiled_tasks_raw_recycle.o fw_util.o -o fw_tiled_tasks -L$(LIBRARY_DIR) $(LIBS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean :
	rm -f fw_rec fw_tiled fw_standard *.o
