build: FORCE
	cd build; make
cmake: FORCE
	mkdir -p build; cd build; cmake ..
run: FORCE
	@build/bin/scnn 'tmp/test_cnn_2.cnn' < 'data/example_input.txt'
test: FORCE
	@build/bin/test
clean: 
	rm -rf build tmp; mkdir tmp; touch tmp/.gitkeep
FORCE:
