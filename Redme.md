- How to run:
  
  on ubuntu install: sudo apt-get install libgtk-3-dev

  then run on root to compile: g++ main.cpp -o calculator.out `pkg-config --cflags --libs gtk+-3.0`
  
  to execute calculator run: ./calculator.out
  