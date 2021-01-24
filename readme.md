# Algolab 2020
## Usage
This section explains my workflow, but of course you can run it however you want.

I compile and run the code in a VM but write the code on the host machine itself, with the code kept in sync using a shared / mounted folder:

1. Install [Vagrant](https://www.vagrantup.com/docs/installation)
2. Clone this repo: `git clone https://github.com/elwin/algolab.git && cd algolab`
3. Start the VM: `vagrant up`
4. SSH into the VM: `vagrant ssh`

Now you can compile and run the code. If it doesn't depend on CGAL, I typically use the following command while inside a specific problem folder:
```bash
g++ -Wall main.cpp -o main && ./main < tests/sample.in
```
to compile and run the with a testfile as input.

If there are dependencies on CGAL, I run
```bash
# Only the first time while in a new problem
cgal_create_cmake_script
cmake .

# Every time after changes were applied
make && ./main < tests/sample.in
```