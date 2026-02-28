install:
	sudo apt install build-essential libxcb1-dev libxcb-randr0-dev

build:
	gcc src/main.c src/xcb_input.c src/xcb_output.c -lxcb -lxcb-randr -o cingle

run:
	./cingle

clean:
	rm cingle
