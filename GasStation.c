//Petrol Pump Solution by MajAK

#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

#define total_cars 100000
#define total_attendes 3
#define max_cars 7
#define total_pumps 3
#define queue_len 4

pthread_t threads[100008];
pthread_t atm;
sem_t max_capacity;
int free_pumps = total_pumps;
sem_t mutex2;
int free_queue_pos=queue_len;
sem_t mutex_queue;
sem_t sem_queue;
sem_t sem_pump;
int pump[total_pumps];
sem_t car_ready;

sem_t finished[total_cars];
sem_t done[total_cars];
sem_t finishedcounter;
sem_t leave_pump[total_pumps];

sem_t mutex3;
sem_t mutex4;

sem_t readywithpayment;

sem_t receipt[total_cars];

int pipe1[2],pipe2[2];

void write_pipe(int writefd,int value)
{
	if(write(writefd,&value,sizeof(int)) != sizeof(int)) {
		printf("pipe write error");
		exit(0);
	}
}


void read_pipe(int readfd,int *value,int *status)
{
	int n;
	if( (n =  read(readfd,value,sizeof(int))) == -1) {
		printf ("pipe read error");
		exit(0);
	}
	*status = (char)n;
}


void Attender(int number);

void ATM();

void Car(int CarId);
void serveCar(int myCar,int number,int myPump);
void acceptPayment();

void CarMaker();

void AttenderMaker();

int main() { 
	int iseed=time(NULL);
	srand(iseed);
	int i;
	sem_init(&max_capacity,0,max_cars);
	sem_init(&mutex2,0,1);
	sem_init(&mutex3,0,1);
	sem_init(&mutex4,0,1);
	sem_init(&mutex_queue,0,1);
	sem_init(&sem_queue,0,queue_len);
	sem_init(&sem_pump,0,total_pumps);
	sem_init(&car_ready,0,0);
	sem_init(&finishedcounter,0,0);
	sem_init(&readywithpayment,0,0);

	for(i=0;i<total_cars;i++)
	{ 
		sem_init(&finished[i],0,0);
		sem_init(&receipt[i],0,0);
	}
	for(i=0;i<total_pumps;i++)
	{
		sem_init(&leave_pump[i],0,0); pump[i]=-1;
	}

	if (pipe(pipe1) < 0) printf("can't create pipe1\n"); 
	if (pipe(pipe2) < 0) printf("can't create pipe2\n"); 

	// pthread_create(&threads[i],NULL,(void *)&ATM,(void *)i);
	AttenderMaker();
	pthread_create(&atm,NULL,(void *)&ATM,NULL);
	CarMaker();
}

void ATM() {
	int Car;
	int MyId = -1;
	int Mychairno = 0; /*we treat the cash counter is a special dummy pump */
	int status;
	while(1) {
		/* wait for a customer to arrive here with money in hand */
		sem_wait(&readywithpayment);

		/* good to see money. let's see who the guy is */
		sem_wait(&mutex4);
		read_pipe(pipe2[0],&Car,&status);
		sem_post(&mutex4);


		printf("ATM: Car %d has arrived with payment. Calling a attender\n",Car);

		sem_wait(&mutex3);

		write_pipe(pipe1[1],MyId);
		write_pipe(pipe1[1],Mychairno);
		sem_post(&mutex3);
		sem_post(&car_ready);

		printf("ATM: Waiting to hear that Car %d has paid to a attender\n",Car);
		sem_wait(&finishedcounter);
		printf("\n\n\n\nPayment started\n\n\n\n\n\n");
		sleep(0.5);
		printf("ATM: Car %d has paid to a attender\n",Car);

		sem_post(&receipt[Car]);
	} 
}

void AttenderMaker()
{
	int i=1;
	while (i <= total_attendes) {
		pthread_create(&threads[i],NULL,(void *)&Attender,(void *)i);
		i++;
	}

}

void CarMaker()
{
	int i=0;
	while (i < total_cars) {
		sleep(rand()%3);
		pthread_create(&threads[i+total_attendes],NULL,(void *)&Car,(void *)i);
		printf("Car %d born\n",i );
		i++;
	}

}

void Attender(int number) {
	int myCar, myPump, status;
	while(1) {

		printf("Attender %d: waiting for a car\n",number);
		/* we sleep till there is there a car on a pump
		   or there is a call from the atm */
		sem_wait(&car_ready);

		/* obtain customerid and pump_no securely */
		sem_wait(&mutex3);

		read_pipe(pipe1[0],&myCar,&status);
		read_pipe(pipe1[0],&myPump,&status);
		sem_post(&mutex3);

		if (myCar != -1) {
			serveCar(myCar,number,myPump);
		}
		else {
			acceptPayment(); 
		}
	} 
}

void acceptPayment()
{
	sem_post(&finishedcounter);
}

void serveCar(int myCar,int number,int myPump)
{
	printf("Attender %d fills gas in Car %d on pump %d\n",number,myCar,myPump);

	printf("\n\n\n\n\nFilling started\n\n\n\n\n\n\n\n");
	sleep(1);
	printf("filling completed\n");
	printf("Filling finished of Car %d by Attender %d on pump %d\n",myCar,number,myPump);

	sem_post(&finished[myCar]);

	sem_wait(&leave_pump[myPump]);

	printf("Attender %d has seen  Car %d leave pump %d and go to ATM\n",number,myCar,myPump);
	/* now sem_post that a pump is free */
	sem_post(&sem_pump);
}
void Car(int CarId) {
	int i,k;

	/* enterStation begins */
	sem_wait(&max_capacity);
	printf("Car %d enters pump\n",CarId);
	/* enterStation ends */
	/* waitInLine starts */
	sem_wait(&mutex2);
	sem_wait(&mutex_queue);
	if ((free_pumps == 0) || (free_queue_pos < queue_len)) {
		sem_post(&mutex_queue);
		sem_post(&mutex2);
		printf("Car %d deciding petrol quality :)\n",CarId);
		sem_wait(&mutex_queue);
		if (free_queue_pos <= 0) {
			printf("Car %d waiting\n",CarId);
		}
		free_queue_pos--;
		sem_post(&mutex_queue);
		sem_wait(&sem_queue); 

		printf("Car %d is in queue now\n",CarId);
		sem_wait(&sem_pump);
		/* waitInLine ends */
		/* goToPump starts */
		sem_wait(&mutex_queue);
		free_queue_pos++;
		sem_post(&mutex_queue);
		sem_post(&sem_queue);
		printf("Car %d released its queue position\n",CarId);
		/* goToPump ends */
	}
	else {
		sem_post(&mutex_queue);sem_post(&mutex2);sem_wait(&sem_pump);
	}
	/* We wait for pump here */
	sem_wait(&mutex2);
	i = 0;
	while ((pump[i] != -1) && (i < total_pumps)) i++;
	if (i == total_pumps) {
		printf("this is not possible! we are here because at least 1 pump empty!");
		exit(0);
	}
	else {
		pump[i] = CarId;
		free_pumps--; 
		printf("Car %d occupies pump %d. free_pumps = %d\n",CarId,i,free_pumps);
	}
	sem_post(&mutex2);

	sem_wait(&mutex3);
	//		Filling started
	printf("Filling for %d started",CarId);
	write_pipe(pipe1[1],CarId);
	write_pipe(pipe1[1],i);
	sem_post(&mutex3);
	sem_post(&car_ready);

	//Filling done
	sem_wait(&finished[CarId]);
	printf("Filling for %d started",CarId);

	sem_wait(&mutex2);
	free_pumps++;
	pump[i] = -1;
	sem_post(&leave_pump[i]);
	sem_post(&mutex2);
	printf("Car %d left pump %d to atm. free_pumps = %d\n",CarId,i,free_pumps); 

	/*want to pay. let's put our id on to a pipe to the ATM*/
	sem_wait(&mutex4);
	write_pipe(pipe2[1],CarId);
	sem_post(&mutex4);
	/* Pay starts */
	printf("Car %d ready to pay\n",CarId);

	sem_post(&readywithpayment);
	sem_wait(&receipt[CarId]);

	printf("Car %d paid\n",CarId);
	sem_post(&done[CarId]);
	/* Pay ends */
	sem_post(&max_capacity);
	if (CarId == (total_cars-1)) {
		for(k=0;k <= (total_cars-1); k++) sem_wait(&done[k]);
		puts("All cars served");
		exit(0);
	}
	else pthread_exit(0);  //exit station
}
