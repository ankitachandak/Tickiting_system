#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

#define NITER 4000
struct s{
int seat_no[2];
int flag;
};
struct t{
int seat_no[2];
int flag;
};
sem_t mutex;
sem_t mutex2;
int count_s = 1,count_t=0,count_gen=0,count_tat=0,seat_count=0,seat_count_t2=0,count_tat_t2=0,count_s_t2 = 1,count_gen_t2=0;
struct s seats[400],seats_t2[400];
struct t seats_tatkal[75],seats_tatkal_t2[75];
int waitlist=0,tatkal=0,waitlist_t2=0,tatkal_t2=0;
FILE *fp1,*fp2;
void * ThreadAdd(void * a)
{
    int i, tmp,random;
	sem_wait(&mutex);
	//srand(time(NULL));
   	random=rand()%100;
	if(seat_count<=300 && count_s <=400)
	{	
		seats[count_s].flag=1;
		count_s++;
		seat_count++;
		count_gen++;
		fprintf(fp1,"\nrequest number=%d seats alloted=%d and random no %d",count_s,seat_count,random);
		seats[count_s].seat_no[0]=seat_count;
		//printf("seats alloted=%d",count_s);
		if(random>50)
			{
			seat_count++;
			count_gen++;
			fprintf(fp1,",%d",seat_count);
			seats[count_s].seat_no[1]=seat_count;
			//printf(",%d",count_s);
			}
	}
	else if(count_s<=400)
		{	
			if(waitlist<=20)
			{	
			seats[count_s].flag=-1;
			count_s++;
			seat_count++;
			count_gen++;
			waitlist++;
			fprintf(fp1,"\n request number=%d waitlist=%d",count_s,waitlist);
			//printf("wait=%d",waitlist);
			if(random==2 && waitlist<20)
				{
				seat_count++;
				count_gen++;
				waitlist++;
				fprintf(fp1,",%d",waitlist);
				seats[count_s].seat_no[1]=seat_count;
				//printf(" ,%d",waitlist);
				}
			}
		else fprintf(fp1,"no seats availabale\n"); 
		}
	sem_post(&mutex);
}
void * ThreadAdd_t2(void * a)
{
    int i, tmp,random;
	sem_wait(&mutex2);
   	random=rand()%100;
	if(seat_count_t2<=300 && count_s_t2<400)
	{	
		seats_t2[count_s_t2].flag=1;
		count_s_t2++;
		seat_count_t2++;
		count_gen_t2++;
		fprintf(fp2,"\n of train2-request number=%d seats alloted=%d and random no %d",count_s_t2,seat_count_t2,random);
		seats_t2[count_s_t2].seat_no[0]=seat_count_t2;
		//printf("seats alloted=%d",count_s_t2);
		if(random>50)
			{
			seat_count_t2++;
			count_gen_t2++;
			fprintf(fp2,",%d",seat_count_t2);
			seats_t2[count_s_t2].seat_no[1]=seat_count_t2;
			//printf(",%d",count_s_t2);
			}
	}
	else if(count_s_t2<400)
		{	
			if(waitlist_t2<=20)
			{	
			seats_t2[count_s_t2].flag=-1;
			count_s_t2++;
			seat_count_t2++;
			count_gen_t2++;
			waitlist_t2++;
			fprintf(fp2,"\n request number=%d waitlist=%d",count_s_t2,waitlist_t2);
			//printf("wait=%d",waitlist);
			if(random==2 && waitlist_t2<20)
				{
				seat_count_t2++;
				count_gen_t2++;
				waitlist_t2++;
				fprintf(fp2,",%d",waitlist_t2);
				seats_t2[count_s_t2].seat_no[1]=seat_count_t2;
				//printf(" ,%d",waitlist_t2);
				}
			}
		else fprintf(fp2,"no seats availabale\n"); 
		}
	sem_post(&mutex2);
}
void * Threadsub(void * a)
{
    int i, tmp,r,f=0;
	sem_wait(&mutex);
	if(seat_count>0 && count_s<=400)
	{	//srand(time(NULL));
		r=rand()%count_s;
		seats[r].flag=0;
		seat_count--;
		fprintf(fp1,"request number=%drservation cancelled\n",r);
		//printf("rservation cancelled request %d\n",r);
		if(waitlist>0)
		{	waitlist--;
			for(i=1;i<=count_s;i++)
			{
				if(seats[i].flag==-1 && f==0)
				{
					seats[i].flag=1;
					seats[i].seat_no[0]=seats[r].seat_no[0];
					fprintf(fp1,"\nrequest number=%d seat %d alloted from waitlist\n",i,seats[i].seat_no[0]);
					//printf("seat %d alloted from waitlist\n",seats[i].seat_no[0]);
					if(seats[r].seat_no[1])
					{	seats[i].seat_no[1]=seats[r].seat_no[1];
						fprintf(fp1," ,%d\n",seats[i].seat_no[0]);
						//printf("seat %d alloted from waitlist same request\n",seats[i].seat_no[0]);
					}
					f=1;
				}
				
			}
		
		}
	}
	else fprintf(fp1,"cancellation unsuccessful\n");
	sem_post(&mutex);
}
void * Threadsub_t2(void * a)
{
    int i, tmp,r,f=0;
	sem_wait(&mutex2);
	if(seat_count_t2>0 && count_s_t2<400)
	{	//srand(time(NULL));
		r=rand()%count_s_t2;
		seats_t2[r].flag=0;
		seat_count_t2--;
		fprintf(fp2,"request number=%drservation cancelled\n",r);
		//printf("rservation cancelled request %d\n",r);
		if(waitlist_t2>0)
		{	waitlist_t2--;
			for(i=1;i<=count_s_t2;i++)
			{
				if(seats_t2[i].flag==-1 && f==0)
				{
					seats_t2[i].flag=1;
					seats_t2[i].seat_no[0]=seats_t2[r].seat_no[0];
					fprintf(fp2,"\nrequest number=%d seat %d alloted from waitlist\n",i,seats_t2[i].seat_no[0]);
					//printf("seat %d alloted from waitlist\n",seats_t2[i].seat_no[0]);
					if(seats_t2[r].seat_no[1])
						{seats_t2[i].seat_no[1]=seats_t2[r].seat_no[1];
						fprintf(fp2," ,%d\n",seats_t2[i].seat_no[0]);
						//printf("seat %d alloted from waitlist same request\n",seats_t2[i].seat_no[0]);
					}
					f=1;
				}
				
			}
		
		}
	}
	else fprintf(fp2,"cancellation unsuccessful\n");
	sem_post(&mutex2);
}

void * ThreadAddtat(void * a)
{
    int i, tmp,random;
	sem_wait(&mutex);
	//srand(time(NULL));
	random=rand()%2+1;
	if(tatkal<=50 && count_tat<=75)
	{
		count_tat++;
		tatkal++;
		seats_tatkal[count_tat].flag=1;		
		fprintf(fp1,"\nrequest number=%d seats alloted in tatkal=%d",count_tat,tatkal);
		seats_tatkal[count_tat].seat_no[0]=tatkal;
		//printf("seats alloted in tatkal=%d",tatkal);
		if(random==2 && tatkal<50)
			{
			tatkal++;
			fprintf(fp1," ,%d",tatkal);
			seats_tatkal[count_tat].seat_no[1]=tatkal;
			//printf("seats alloted same request =%d",tatkal);
			}
	}
	else fprintf(fp1,"seats unavailable in tatkal");
	sem_post(&mutex);
}
void * ThreadAddtat_t2(void * a)
{
    int i, tmp,random;
	sem_wait(&mutex2);
	random=rand()%2+1;
	if(tatkal_t2<=50 && count_tat_t2<=75)
	{
		//printf("In tatkal");
		count_tat_t2++;
		tatkal_t2++;
		seats_tatkal_t2[count_tat_t2].flag=1;		
		fprintf(fp2,"\nrequest number=%d seats alloted in tatkal=%d",count_tat_t2,tatkal_t2);
		seats_tatkal_t2[count_tat_t2].seat_no[0]=tatkal_t2;
		//printf("seats alloted in tatkal=%d",tatkal_t2);
		if(random==2 && tatkal_t2<50)
			{
			tatkal_t2++;
			fprintf(fp2," ,%d",tatkal_t2);
			seats_tatkal_t2[count_tat_t2].seat_no[1]=tatkal_t2;
			//printf("seats alloted same request =%d",tatkal_t2);
			}
	}
	else fprintf(fp2,"seats unavailable in tatkal");
	sem_post(&mutex2);
}


int main(int argc, char * argv[])
{   
    pthread_t normal1, tatkal, cancellation,normal1_t2, tatkal_t2, cancellation_t2;
	int select,select_t2,day=0,i,day2=0;
	FILE *fp_c1,*fp_c2;
    fp1=fopen("dailystatus","a");
    fp2=fopen("dailystatus_train2","a");
    sem_init(&mutex,0,1);
    sem_init(&mutex2,0,1);
	clock_t start1 = clock();
	while((((double)clock() - start1) / CLOCKS_PER_SEC)<12.0)
  {  	
	select=rand()%4;
	if((((double)clock() - start1) / CLOCKS_PER_SEC) == day)
	{
		fprintf(fp1,"\n\n******* day %d *******\n\n",day+1);
		day++;
	}
	//fprintf(fp1,"The random number",select);	
	switch (select)
	{
	case 0:  if(pthread_create(&normal1, NULL, ThreadAdd, NULL))
		    {
		      printf("\n ERROR creating thread 1");
		      exit(1);
		    }
		 if(pthread_join(normal1, NULL))	/* wait for the thread 1 to finish */
		    {
		      printf("\n ERROR joining thread");
		      exit(1);
		    }
		break;

   
    	case 1:if(pthread_create(&cancellation, NULL, Threadsub, NULL))
		    {
		      printf("\n ERROR creating thread 2");
		      exit(1);
		    }
		 if(pthread_join(cancellation, NULL))        /* wait for the thread 2 to finish */
		    {
		      printf("\n ERROR joining thread");
		      exit(1);
		    }
		break;
	case 2:if((((double)clock() - start1) / CLOCKS_PER_SEC)>=11.52)
		{
			if(pthread_create(&tatkal, NULL, ThreadAddtat, NULL))
			    {
			      printf("\n ERROR creating thread 1");
			      exit(1);
			    }
			 if(pthread_join(tatkal, NULL))        /* wait for the thread 3 to finish */
	   	 	    {
		      		printf("\n ERROR joining thread");
		      		exit(1);
	   	 	    }
		}
		break;
	case 3:  if(pthread_create(&normal1, NULL, ThreadAdd, NULL))
		    {
		      printf("\n ERROR creating thread 1");
		      exit(1);
		    }
		 if(pthread_join(normal1, NULL))	/* wait for the thread 1 to finish */
		    {
		      printf("\n ERROR joining thread");
		      exit(1);
		    }
		break;
	
	
	default: break;
   
  }  
	if((((double)clock() - start1) / CLOCKS_PER_SEC) == day2)
	{
		fprintf(fp2,"\n\n******* day %d *******\n\n",day2+1);
		day2++;
	}
	select_t2 = rand()%4;
	switch(select_t2){
	
	case 0: if(pthread_create(&normal1_t2, NULL, ThreadAdd_t2, NULL) )
		    {
		      printf("\n ERROR creating thread 1");
		      exit(1);
		    }
		 if(pthread_join(normal1_t2, NULL))	/* wait for the thread 1 to finish */
		   {
		      printf("\n ERROR joining thread");
		      exit(1);
		    }
		break;
	case 1:if(pthread_create(&cancellation_t2, NULL, Threadsub_t2, NULL))
		    {
		      printf("\n ERROR creating thread 2");
		      exit(1);
		    }
		 if(pthread_join(cancellation_t2, NULL))        /* wait for the thread 2 to finish */
		  {
		      printf("\n ERROR joining thread");
		      exit(1);
		    }
		break;
	case 2:if((((double)clock() - start1) / CLOCKS_PER_SEC)>=11.52)
		{
			if(pthread_create(&tatkal_t2, NULL, ThreadAddtat_t2, NULL))
			    {
			      printf("\n ERROR creating thread 1");
			      exit(1);
			    }
			 if(pthread_join(tatkal_t2, NULL))        /* wait for the thread 3 to finish */
	   	 		{
		      		printf("\n ERROR joining thread");
		      		exit(1);
	   	 	    }
		}
		break;
	case 3:if(pthread_create(&normal1_t2, NULL, ThreadAdd_t2, NULL))
		    {
		      printf("\n ERROR creating thread 1");
		      exit(1);
		    }
		 if(pthread_join(normal1_t2, NULL))	/* wait for the thread 1 to finish */
		    {
		      printf("\n ERROR joining thread");
		      exit(1);
		    }
		break;
	default: break;
}

		
}
fp_c1=fopen("final_chart1","w");
fp_c2=fopen("final_chart2","w");
for(i=0;i<400;i++)
{
	if(seats[i].flag == 1)
	{
		fprintf(fp_c1,"\nrequest no = %d, seat no = %d  ",i,seats[i].seat_no[0]);
		if(seats[i].seat_no[1])
			fprintf(fp_c1,", %d ",i,seats[i].seat_no[1]);		
	}
			
	if(seats_t2[i].flag == 1)
	{
		fprintf(fp_c2,"\nrequest no = %d, seat no = %d  ",i,seats_t2[i].seat_no[0]);
		if(seats_t2[i].seat_no[1])
			fprintf(fp_c2,", %d ",i,seats_t2[i].seat_no[1]);		
	}
}
for(i=0;i<75;i++)
{
	if(seats_tatkal[i].flag == 1)
	{
		fprintf(fp_c1,"\nrequest no = %d, seat no = %d  ",i,seats_tatkal[i].seat_no[0]);
		if(seats_tatkal[i].seat_no[1])
			fprintf(fp_c1,", %d ",i,seats_tatkal[i].seat_no[1]);		
	}
			
	if(seats_tatkal_t2[i].flag == 1)
	{
		fprintf(fp_c2,"\nrequest no = %d, seat no = %d  ",i,seats_tatkal_t2[i].seat_no[0]);
		if(seats_tatkal_t2[i].seat_no[1])
			fprintf(fp_c2,", %d ",i,seats_tatkal_t2[i].seat_no[1]);		
	}
}
    pthread_exit(NULL);

}


