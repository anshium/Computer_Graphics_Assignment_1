# Report for CG Assignment - 1

## Question 1

<b>Task:</b> To find out problems with the file incorrect.json and the object file associated with it.

<b>The problem:</b> The floating points usually have a trade-off. They can often represent very large numbers. But in those cases, the precision of the numbers decreases. The number 10 lakh (10,00,000) is a number with which that problem arises.

<b>The fix:</b> Doubles trypically have 2x more precision than floats.

I initially changed some floats to doubles in certain parts of the function headers and elsewhere, but later settled with changing to `double` from `float` in the declearion of `Vector3f`.

## Question 2

### Scene_1
1. <b>Lo_Poly:</b>
	- 0: `76674.281250 ms`
	- 1: `20992.304688 ms`
	- 2: `6007.235840 ms`
	- 3: `353.445007 ms`
2. <b>Hi_Poly:</b>
	- 0: `331928.000000 ms`
	- 1: `87774.179688 ms`
   	- 2: `23035.443359 ms`
	- 3: `377.324005 ms`

### Donuts and Glass and Table
	
- 0: `100826.453125 ms`
- 1: `27691.812500 ms`
- 2: `8065.392090 ms`
- 3: `402.031006 ms`

### Monkeys:
- 0: `80586.937500 ms`
- 1: `20720.361328 ms`
- 2: `6159.920898 ms`
- 3: `156.770996 ms`

<b>Note:</b> Some timings maybe longer due to an I/O overhead arising out of certain print statements added across the code.