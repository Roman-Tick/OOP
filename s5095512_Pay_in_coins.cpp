#include <stdio.h>
#include <stdlib.h>
//sometimes only works when there is less than 4 lines in the input file

int **read_file(int *row) //reads numbers from the input text file
{
	FILE *input_file = NULL;
	input_file = fopen("input.txt", "r"); // change file name here
	
	if (input_file == NULL) //checks if file is able to be read
	{
		printf("unable to open input text file. :( \nput the input text file in the same directory as the .exe \n");
	}
	
	int col=0;
	int **numbers = (int **) malloc(1 * sizeof(int)); // allocates memory block for array
	if (*numbers == NULL)
	{
		printf("memory for numbers array could not be allocated");
	}
	numbers[*row] = (int *) malloc(3 * sizeof(int)); //allocates a memory block for the inside array (eg. an array within each row)
	numbers[*row][0]=0; //sets undefined values to 0
	numbers[*row][1]=0;
	numbers[*row][2]=0;
	char ch;
	while (fscanf(input_file, "%c", &ch) == 1)
	{
		if ( ( (int)ch) == 10) //checking for new line characters
		{
			(*row)++;
			int *tmp_ptr = (int*)realloc(*numbers, ((*row) + 1) * sizeof(int)); //allocates new larger memory block. copies the array in numbers and adds it to this address instead. it then frees where numbers old address was 
			if (tmp_ptr == NULL)
			{
				printf("could not create extra memory block for numbers (using realloc) from text file");
			}
			else
			{
				*numbers = tmp_ptr; // sets numbers to the same address as tmp_ptr
			}
			numbers[*row] = (int *) malloc(3 * sizeof(int)); //creates an array within the row
			numbers[*row][0]=0; // calloc didnt work so had
			numbers[*row][1]=0; // to use malloc and initialize 
			numbers[*row][2]=0; // each different block as zero
			col=0;
		}
		else if ( ( (int)ch) > 47 && ( (int)ch) < 58) //checking for number characters
		{
			numbers[*row][col] = (numbers[*row][col] * 10) + ((int)ch-48); //puts all ints in array but doesnt count for new line
		}
		else if ((int)ch == 32) //checking for spaces
		{
			col++;
		}
		
	}
	col=0;
	
	/*
	for(int i=0;i<=(*row);i++) //prints numbers in the file
	{
		while (col<=2)
		{
			printf("%d ", numbers[i][col]);
			col++;
		}
		col=0;
		printf("\n");
	} */
	
	fclose(input_file);
	return numbers;
}

int *get_primes(int gold_coin, int *index) //finds all prime numbers below the gold coin
{
	int *primes = (int*)malloc(1*sizeof(int)); //creates a dedicated memory block for the array of prime numbers
	if(primes == NULL)
	{
		printf("Memory for primes could not be allocated");
	}
	
	(*index) = 0; //keeps track of which index (in the primes array) to place the number in
	
	for(int i=1;i<gold_coin;i++) //for every number below the gold coin, check whether they are prime
	{
		bool is_prime=true; //flag to see if the number is prime 
		for(int b=2;b<=(i/2);b++) //it is not a prime number if i is divisible but any number between 2 and b/2 
		{
			if(i%b == 0) //is not a prime number
			{
				is_prime = false;
			}
		}
		
		if(is_prime == true) //if prime number is found
		{
			primes[*index] = i; //adds the number to the primes array
			(*index)++;
			int *tmp; //creating a temporary pointer in resize the primes array
			tmp = (int*)realloc(primes, ((*index)+1) * sizeof(int)); //creates a memory block plus one the size of the primes array/primes memory block
			if(tmp == NULL)
			{
				printf("primes resize has failed");
			}
			else
			{
				primes = tmp; //sets the address of the primes array to the same address as tmp. this basically increases the size of the primes array by one
			}
		}
	}
	
	return primes; //returns the primes array to the main function
}

void sum_of_primes(int index,int *num_of_combs,int *primes,int gold_coin,int element,int *prime_combinations,int limit,int Ncoins_r1, int r2)
{
	int sum = 0, amount_in_combination = 0;
	prime_combinations[element] = primes[index];
	for(int i=0;i<gold_coin;i++) //checks the sum each time the function is entered
	{
		sum += prime_combinations[i];
	}
	
	if(sum == gold_coin)
	{
		for(int i=0;i<gold_coin;i++)
		{
			if(prime_combinations[i] != 0)
			{
				amount_in_combination++;
			}
		}
		
		if(Ncoins_r1 == 0 && r2 == 0) //checking the constaints
		{
			(*num_of_combs)++;
		}
		else
		{
			if(r2 == 0)
			{
				if(amount_in_combination == Ncoins_r1)
				{
					(*num_of_combs)++;
				}
			}
			else
			{
				if(amount_in_combination >= Ncoins_r1 && amount_in_combination <= r2)
				{
					(*num_of_combs)++;
				}
			}
		}
		
		/*   //this comment section is for printing the correct combinations of prime numbers (for debugging purposes)
		printf("|");
		for(int i=0;i<gold_coin;i++) 
		{
			printf("%d ", prime_combinations[i]);
		}
		printf("|\n");
		*/
		
		prime_combinations[element] = 0;
		return;
	}
	else if(sum > gold_coin)
	{
		
		prime_combinations[element] = 0;
		return;
	}
	else
	{
		
		sum_of_primes(index, num_of_combs, primes, gold_coin, (element + 1), prime_combinations,limit,Ncoins_r1,r2); // calls function again
		
		if((index + 1) != (limit))
		{
			sum_of_primes((index+1), num_of_combs, primes, gold_coin, (element), prime_combinations,limit,Ncoins_r1,r2);
			prime_combinations[element] = 0;
			element--;
			
		}
		else
		{
			index -= limit;
			limit--;
		}
		
		return;
	}
	
}

int combinations(int gold_coin, int Ncoins_r1, int r2, int *primes, int size, int *num_of_combs) //finds how many combinations in prime number array (primes) add up to the gold coin
{
	(*num_of_combs) = 0; //number of combinations
	int index_num=0, element=0;
	
	int prime_combinations[gold_coin] = {0}; // array of prime combinations. gold coins is the limit because you will a combinations bigger than gold coin * 1 (eg. 1,1,1,1.....)
	
	sum_of_primes(index_num, num_of_combs, primes, gold_coin, element, prime_combinations,size,Ncoins_r1,r2);
	
	if(Ncoins_r1 == 0) // checks whether there is any constrants on the combinations so the output is formatted correctly
	{
		printf("Total number of ways to pay $%d = %d\n\n", gold_coin, (*num_of_combs+1));
	}
	else
	{
		if(r2 == 0)
		{
			if(Ncoins_r1 == 1)
			{
				printf("Total number of ways to pay $%d (using %d combinations) = %d\n\n", gold_coin, Ncoins_r1, (*num_of_combs+1));
			}
			else
			{
				printf("Total number of ways to pay $%d (using %d combinations) = %d\n\n", gold_coin, Ncoins_r1, *num_of_combs);
			}
		}
		else
		{
			if(Ncoins_r1 == 1)
			{
				printf("Total number of ways to pay $%d (with range of %d - %d) = %d\n\n", gold_coin, Ncoins_r1, r2, (*num_of_combs+1));
			}
			else
			{
				printf("Total number of ways to pay $%d (with range of %d - %d) = %d \n\n", gold_coin, Ncoins_r1, r2, *num_of_combs);
			}
		}
	}
}

int main()
{
	int lines=0,number_of_primes=0,number_of_combinations;
	int **input = read_file(&lines); //fuction to that reads all numbers in the input file
	
	for(int i=0;i<=lines;i++) //for each line in the input file, it calcutes the combination sum
	{
		int *primes = get_primes(input[i][0],&number_of_primes);
		
		/*
		for(int j=0;j<(number_of_primes);j++) // this comment section is for printing array of prime numbers less than the gold coin
		{
			printf("| %d ", primes[j]);
		}
		printf("\n");
		*/
		
		combinations(input[i][0], input[i][1], input[i][2], primes, number_of_primes, &number_of_combinations);
	}
}
