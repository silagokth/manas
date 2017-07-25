# Manus
 
A SiLago assembler

## Input and Output

The input of Manus is a caseless text file which contains the defination of variables and
the list of instructions. The output is a testbench used for simulation under
QuestaSim.

## Grammar

The input file is a text file. The content is divided into many segments by __SEGMENT INSTRUCTIONS__.
The _data segment_ is used for defining variables while the _code segment_ is used
for defining the instruction list.

### Segment instructions

````.DATA````

start a data segment

````.CODE````

start a code segment

### Define a variable

````$variable_name distribution_type [<row0, col0>, <row1, col1>, ...] [element0, element1, ...]````

Variable name should always be led by a __$__ symbol.

Distribution type can be either __FULL\_DISTR__ or __EVEN\_DIRSR__.

The third argument is a list of DRRA cells.

The last argument is the initial value of each element inside the variable.

All variable are considered as a 1-D array.

### Define instructions

````instruction_name arg0 arg1 ...````

instruction name can be __DPU__, __REFI1__, __REFI2__, ... For the full list and their detailed
argument description, please read the documentation of Vesyla.

## Sample code from the test001_01

````
.DATA
$A EVEN_DISTR [<0,0>] ONES(10)
$B EVEN_DISTR [<0,0>] ONES(10)
$C FULL_DISTR [<0,0>] ONES(1)

.CODE
CELL <0,0>
SWB REFI 0 3 LATA 0 2
SWB REFI 0 2 LATA 0 3
SWB LATA 0 0 REFI 0 1
REFI_1 3 0 0 0 0 0 0 2
REFI_1 2 0 0 10 0 0 0 1
DPU 10 0 3 3 1 0 0 0
REFI_1 1 0 0 20 0 0 0 0

````
