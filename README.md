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

or

````$variable_name distribution_type [<row0, col0>, <row1, col1>, ...] ZEROS(number_of_element)````

or

````$variable_name distribution_type [<row0, col0>, <row1, col1>, ...] ONES(number_of_element)````

Variable name should always be led by a dollar (__$__) symbol. The first symbol after dollar (__$__) should only be underscore ( **\_** ) or regular latin letter (a-z, A-Z). The following symbol can be underscore ( **\_** ), regular latin letter (a-z, A-Z) as well as number (0-9).

Distribution type can be either __FULL\_DISTR__ or __EVEN\_DIRSR__.

The third argument is a list of DRRA cells.

The last argument is the list of initial value of each element inside the variable.

All variable are considered as a 1-D array in register file.

### Define instructions

````instruction_name arg0 arg1 ...````

or

````instruction_name arg0, arg1, ...````

or

````instruction_name (arg0, arg1, ...)````

instruction name can be __DPU__, __REFI1__, __REFI2__, ... For the full list and their detailed
argument description, please read the documentation of Vesyla.

## Sample code from the test001_01

````
.DATA
$A EVEN_DISTR [<0,0>] [3,4,5,6,7,8,9,10,11,12]
$B EVEN_DISTR [<0,0>] [5,6,7,8,9,10,11,12,13,14]
$C FULL_DISTR [<0,0>] [0]

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

## Limitations
- Doesn't support fixed-point representation, you need to convert fixed-point to integer format manually.
- Doesn't support DiMarch variable declaration.
- Doesn't support Input and Output variable type, only Temporary variable type is valid.
- No static time model analysis. The execution cycle is by default set to 1000.

