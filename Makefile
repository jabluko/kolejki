PRG_C = $(wildcard *.cpp)
OBJ_C   = $(patsubst %.cpp,%.o,$(PRG_C))
CFLAGS = @opcjeCpp

out.e: $(OBJ_C)
	g++ $(CFLAGS) $(OBJ_C) -o $@

%.o: %.cpp
	g++ -c $(CFLAGS) $< -o $@
	g++ -MM $(CFLAGS) $< > $(patsubst %.c,%.d,$<)

clean:
	rm -f *.e *.o *.d

-include $(OBJ_C:%.o=%.d)
