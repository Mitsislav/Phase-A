/*
 * ============================================
 * file: main.c
 * @Author John Malliotakis (jmal@csd.uoc.gr)
 * @Version 24/10/2023
 *
 * @e-mail hy240@csd.uoc.gr
 *
 * @brief Main function
 *        for CS240 Project Phase 1,
 *        Winter Semester 2024-2025
 * @see   Compile using supplied Makefile by running: make
 * ============================================
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "elections.h"

/* Maximum input line size */
#define MAX_LINE 1024

/* 
 * Uncomment the following line to
 * enable debugging prints
 * or comment to disable it
 */
/* #define DEBUG */
#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else
#define DPRINT(...)
#endif /* DEBUG */

/*
 * Globals:
 * you may add some here for certain events
 * (such as D and P)
 */
int countD=0;int countP=0;

struct district Districts[56];
struct party Parties[5];
struct parliament Parliament;

/* Event A */

void announce_elections(){
    	int i;
			
	/* Initialize the global variables */

    	for (i = 0; i < 56; i++) {
        	Districts[i].did = -1;  
        	Districts[i].seats = -1;
        	Districts[i].allotted = -1;
        	Districts[i].blanks = -1;
        	Districts[i].voids = -1;
        	Districts[i].stations = NULL;
        	Districts[i].candidates = NULL;
    	}

    	for (i = 0; i < 5; i++) {
        	Parties[i].pid = -1;
        	Parties[i].nelected = -1;
        	Parties[i].elected = NULL;
    	}

    	Parliament.members = NULL;

	/* Print */

    	printf("A\nDONE\n");

	return;
}

/* Event D */

int create_district(int did, int seats){
	int i;

	if (countD==56) return 1;	/* if cound of districts is greater than 56 then failed*/

	struct district newDistrict;
					/* initialize the new district with desired values */
	newDistrict.did=did;
	newDistrict.seats=seats;
	newDistrict.allotted=0;
	newDistrict.blanks=0;
	newDistrict.voids=0;
	newDistrict.stations=NULL;
	newDistrict.candidates=NULL;

	Districts[countD]=newDistrict; /* add it to the global array with districts */
	countD++;			/* increase the counter by 1 */

	/* Print */

	printf("D %d %d\n", did,seats);
    	printf("Districts = ");
    	for (i = 0; i <countD; i++) {
        	printf("%d", Districts[i].did);
        	if (i < countD- 1) {
            		printf(", ");
        	}
    	}
    	printf("\nDONE\n");

	return 0;		/* returns success */
}

/* Event S */

int create_station(int sid, int did){
	
	int i;
	struct district * myTarget=NULL;
	struct station * current=NULL;
	
	/* Searching for the right district to add the new station */

	for(i=0;i<countD;i++){
		if(Districts[i].did==did)
			myTarget=&Districts[i];
	}

	if(myTarget==NULL) return 1;	/* not found the district then return false */

	struct station* newStation=malloc(sizeof(struct station));	/* allocate memory for station */

	newStation->sid=sid;
	newStation->registered=0;
	newStation->next=NULL;
	
	newStation->vsentinel=malloc(sizeof(struct voter));	/* initialize the guardian node with illegal vid */
	newStation->vsentinel->vid=-1;	/* invalid voter id */

	newStation->voters=newStation->vsentinel;	/* the header of voters list is empty (guardian node) */

	if(myTarget->stations==NULL){			/* district is empty of stations */
		myTarget->stations=newStation;		/* the header of station list becomes the new station */
	}else{
		current=myTarget->stations;
		while(current->next!=NULL){		/* navigate throgh the list to reach the last element */
			current=current->next;
		}
		current->next=newStation;		/* add to the tail of list the new station */
	}

	/* Print */

	printf("S %d %d\n",sid,did);
    	printf("Stations = ");
    	
	current = myTarget->stations;
    	while (current != NULL) {
        	printf("%d", current->sid);
        	if (current->next != NULL) {
            		printf(", ");
        	}
        	current = current->next;
    	}	

    	printf("\nDONE\n");

	return 0;
}

/* event P */

void create_party(int pid){
	int i;

	if(countP==5) return;	/* if party counter is over 5 then return (fail) */

	Parties[countP].pid=pid;
	Parties[countP].nelected=0;
	Parties[countP].elected=NULL;

	countP++;

	/* Print */

	printf("P %d\n", pid);
    	printf("Parties = ");
    	for (i = 0; i <countP; i++) {
        	printf("%d", Parties[i].pid);
        	if (i < countP - 1) {
            		printf(", ");
        	}
    	}
    	printf("\nDONE\n");

	return;
}

/* event C */

int register_candidate(int cid, int did, int pid){
	int i;
	struct district * myTarget=NULL;
	struct candidate * current=NULL;

	for(i=0;i<countD;i++){	/* check in if district with did id exists */
		if(Districts[i].did==did)
			myTarget=&Districts[i];
	}

	if(myTarget==NULL) return 1; /* does not exist then return 1 (fail) */

	struct candidate * newCandidate=malloc(sizeof(struct candidate));	/* allocate memory for new candidate */
				
	newCandidate->elected=0;	/* initialize the new candidate with desired values for each field */
	newCandidate->votes=0;
	newCandidate->cid=cid;
	newCandidate->pid=pid;
	newCandidate->next=NULL;
	newCandidate->prev=NULL;

	if(myTarget->candidates==NULL){	/* candidates list of district is empty */
		myTarget->candidates=newCandidate;	/* becomes the new header of candidates list */
	}else{
		current=myTarget->candidates;	
		while(current->next!=NULL){	/* navigate through the list and reach the last one */
			current=current->next;
		}
		current->next=newCandidate;	/* add the new one to the tail */
		newCandidate->prev=current;	/* prev pointer looks to the previous elem */
	}

	/* Print */
	
	printf("C %d %d %d\n",cid,did,pid);
    	printf("Candidates = ");
    	
	current = myTarget->candidates;
    	while (current != NULL) {
        	printf("%d", current->cid);
        	if (current->next != NULL) {
            		printf(", ");
        	}
        	current = current->next;
    	}	

    	printf("\nDONE\n");

	return 0;
}

/* event R */

int register_voter(int vid, int did, int sid){
	int i;
	struct district * myTarget=NULL;
				
	for(i=0;i<countD;i++){	/* check if district with did id exists */
		if(Districts[i].did==did)
			myTarget=&Districts[i];
	}

	if(myTarget==NULL) return 1; /* does not exist and returns 1 (fail) */

	struct station * myStation=myTarget->stations;
	
	while(myStation!=NULL){		/* checks if station with sid id exists */
		if(myStation->sid==sid)
			break;
		myStation=myStation->next;
	}

	if(myStation==NULL) return 1; /* does not exists and returns 1 (fail) */

	struct voter * newVoter=malloc(sizeof(struct voter)); /* allocate memory for the new voter */

	newVoter->vid=vid;
	newVoter->next=NULL;
	newVoter->voted=0;

	struct voter * current;	 /* if the header of the list is the guardian then replace it with newVoter */
	if(myStation->voters==myStation->vsentinel){
		myStation->voters=newVoter;
		myStation->voters->next=myStation->vsentinel; /* new header looks to the guardian node */
	}else{
		current=myStation->voters;
		while(current->next!=myStation->vsentinel){ /* pass through the list until find guardian node */
			current=current->next;
		}
		current->next=newVoter;	/* adds the new voter at the end of the list */
		newVoter->next=myStation->vsentinel; /* the last one shows to guardian node again */
	}

	myStation->registered++;

	/* Print */

	printf("R %d %d %d\n", vid,did,sid);
    	printf("Voters = ");

    	current = myStation->voters;
    	while (current != myStation->vsentinel) {
        	printf("%d", current->vid);
        	if (current->next != myStation->vsentinel) {
            		printf(", ");
        	}
        	current = current->next;
    	}

    	printf("\nDONE\n");

	return 0;
}

/* event U */

int unregister_voter(int vid){

	int i;
	struct district * myDistrict;
	struct station * myStation;
	struct voter * current;
	struct voter * prev;

	for(i=0;i<countD;i++){
		
		myDistrict=&Districts[i];	/* taking the first district */
		myStation=myDistrict->stations; /* taking the first station of the selected district */

		while(myStation!=NULL){
			
			current=myStation->voters;	/* taking the voters list for this specific voting station */
			prev=NULL;			/* prev must be null each time */

			while(current!=myStation->vsentinel){	/* until reach the end of the list (guardian node for voters) */
				
				if(current->vid==vid){
					
					if(prev==NULL)
						myStation->voters=current->next;	/* means voter we looking for is the first of the list */
					else
						prev->next=current->next;	/* link the previous node with the next node exclude the unregistered voter */

					myStation->registered--;	/* registered counter decrease by one */

					/* Print */
					
					printf("U %d\n", vid);
					printf("%d %d\n", myDistrict->did, myStation->sid);
					printf("Voters = ");

					current=myStation->voters;

					while(current!=myStation->vsentinel){
						printf("%d",current->vid);
						if(current->next!=myStation->vsentinel)
							printf(", ");
						current=current->next;
					}
					printf("\nDONE\n");

					//free(current);

					return 0;
				}
			
				prev=current;	/* store the current node as the prev */
				current=current->next;	/* move on with the next element of our lsit */
			}
			myStation=myStation->next;	/* not found so go on with the next station of this district */	
		}
	}
	return 1;
}

/* Event E */

void delete_empty_stations(void){

	struct district * currentDistrict;
	struct station * currentStation;
	struct station * prevStation;
	int i;
	
	printf("E\n");

	for(i=0;i<countD;i++){

		currentDistrict=&Districts[i];	/* take the first district */
		currentStation=currentDistrict->stations;/* takes the first station of each district */
		prevStation=NULL;

		while(currentStation!=NULL){
			
			if(currentStation->registered==0){	/* if station has zero registered voters then delete this station */

				if(prevStation==NULL){
					currentDistrict->stations=currentStation->next;	/* if prev is null then the new header of specific district voters list is the next station */
				}else{
					prevStation->next=currentStation->next; /* if not then link the prev station to the next station of the current */
				}
				
				/* Print */
				
				printf("%d %d \n",currentStation->sid,currentDistrict->did);

				/* free the memmory */
				//struct station * forFree=currentStation;
				//free(forFree);
			}
			prevStation=currentStation;	/* prev station is the current */
			currentStation=currentStation->next;	/* current station keeps going with the next one */
		}
	}
	printf("DONE\n");

	return;
}

/* Event V */

void sortDLL(int); /* just the prototype of the function using in int vote() */

int vote(int vid, int sid, int cid){

	struct district * myDistrict=NULL;
	struct station * myStation=NULL;
	struct voter * myVoter;
	int i;

	for(i=0;i<countD;i++){
	
		myDistrict=&Districts[i];	/* starting taking the first district of the array */
		myStation=myDistrict->stations;	/* takes the first station of this specific district */
		
		while(myStation!=NULL){
			
			if(myStation->sid==sid){	/* if station sid is the one we looking for then break */
				//printf("Station %d found in district %d (%p %p)\n", sid, myDistrict->did, myStation, myDistrict);
				break;
			}
			myStation=myStation->next;	/* if not just keep looking */
		}
		if(myStation!=NULL) 
			break;
		else
			continue;
			//printf("Station with sid:%d id in district %d:did does not exist\n",sid,myDistrict->did);
	}	
		
	if(myStation==NULL){	 //if station with sid id does not exists return one 
		printf("Station with %d sid does not exist in district did:%d\n",sid,myDistrict->did);	
		return 1; 		
	}
	//printf("Station is %d, district is %d (%p %p)\n", myStation->sid, myDistrict->did, myStation, myDistrict);
	/* */
	myVoter=myStation->voters;	/* takes the first voter of the list of the station */
	while(myVoter!=myStation->vsentinel){	/* until we reach the guardian node (tale of the linked list) */
		if(myVoter->vid==vid) break;	/* if find the voter with vid id then break */
		myVoter=myVoter->next;		/* if not then keep looking for him */
	}

	if(myVoter==myStation->vsentinel){
		printf("Voter with %d vid does not exist\n",vid);
		return 1;	/* voter with vid id does not exist */
	}

	if(myVoter->voted==0)
		myVoter->voted=1;	/* this voter has voted */
	else{
		printf("Voter :%d has already voted\n",myVoter->vid);
		return 1;
	}
		

	int key=0;

	if(cid==0){	/* if cid is zero then his vote is blank */
		myDistrict->blanks++;
		key=1;
		if(myDistrict==NULL)
			printf("%d district is null\n",myDistrict->did);
	}	
	if(cid==1){	/* if cid is one then his vote is void */
		myDistrict->voids++;
		key=1;	
	}
	//printf("vid:%d , sid:%d , cid:%d\n",vid,sid,cid);

	
	struct candidate * currentCandidate=myDistrict->candidates; /* taking the first candidate of the list */
	int found=0;	/* checks if candidate found */

	while(currentCandidate!=NULL && key!=1){	/* if cid is 0 or 1 not */
		if(currentCandidate->cid==cid){	/* if cid if found*/
			currentCandidate->votes++;	/* increase his candidate votes */
			sortDLL(i);	/* sort the candidate list of specific */
			found=1;	/* that means found him */
			break;
		}
		currentCandidate=currentCandidate->next;	/* keep going with next one*/
	}
	
	if(found==0 && key==0){
		printf("Candidate not found %d at this district:%d\n",cid,myDistrict->did);
		return 1;	/* if cid is not 0 or 1 and previous loop didnt find him return 1*/
	}

	/* Print */

	printf("V <%d> <%d> <%d>\n", vid, sid, cid);
    	printf("District = %d\n", myDistrict->did);
    	printf("Candidate votes = ");
    	currentCandidate = myDistrict->candidates;

    	while (currentCandidate != NULL) {
        	printf("(<%d>, <%d>)", currentCandidate->cid, currentCandidate->votes);
        	currentCandidate = currentCandidate->next;
        	if (currentCandidate != NULL) 
			printf(", ");
		else
			printf("\n");
    	}

    	printf("Blanks = <%d>\n", myDistrict->blanks);
    	printf("Voids = <%d>\n", myDistrict->voids);
   	printf("DONE\n");

	return 0;
}

/* sort double linked list by votes */

void sortDLL(int i){
  
	/* taking the ith district candidate list and put him as the header */
    	struct candidate *head = Districts[i].candidates;

    	if (head == NULL) return;	/* if header is null just return*/

    	struct candidate *sorted=NULL;	/* the new sorted list is empty */
    	struct candidate *current=head;	/* starting with the head of the list */

    	while (current != NULL) {
        	
		struct candidate *next = current->next;	/* stores the next of current */
        	if(sorted == NULL || sorted->votes <= current->votes){
                    	current->next = sorted;	/* putting current as the first element of sorted list */
            		if (sorted != NULL) {
                		sorted->prev = current;
            		}
            		sorted=current;
            		sorted->prev=NULL;  
        	}else{	/* finding the right place to place him */
    			struct candidate * current2=sorted;
            		while (current2->next != NULL && current2->next->votes > current->votes) {
                		current2=current2->next;
            		}
            		current->next=current2->next;
            		if (current2->next != NULL) {
                		current2->next->prev=current;
            		}

            		current2->next=current;
            		current->prev=current2;
        	}

        	current = next; 	
	}
	/* then the candidates list is sorted */
    	Districts[i].candidates = sorted;
	return;	
}

/* Event M */

void count_votes(int did){
	
	struct district * myDistrict;
	int i;
	for(i=0;i<countD;i++){	/* looking for the district with did id */

		myDistrict=&Districts[i];
		if(myDistrict->did==did)
			break;
	}
	if(i==countD){	/* if i=countD that means did district didnt found */
		printf("District with did:%d does not exist\n",did);
		return;
	}
/*
	int all=0;
	for(i=0;i<countD;i++){
		all+=Districts[i].seats;
	}
	printf("This district has:%d\n",all); */

	/* 1 */

	int helperParty[5]={0,0,0,0,0};

	struct candidate * current=myDistrict->candidates;

	while(current!=NULL){
		if(current->pid == Parties[0].pid){
			helperParty[0]+=current->votes;
		}else if(current->pid == Parties[1].pid){
			helperParty[1]+=current->votes;
		}else if(current->pid == Parties[2].pid){
			helperParty[2]+=current->votes;
		}else if(current->pid == Parties[3].pid){
			helperParty[3]+=current->votes;
		}else if(current->pid == Parties[4].pid){
			helperParty[4]+=current->votes;
		}else if(current->pid==0 || current->pid==1){
			printf("its void or blank\n");
		}else{
			printf("something went wrong\n");
			return;
		}
		current=current->next;
	}


	
	/* 2 */

	int sumSeats=myDistrict->seats;
	int sumVotes=0;
	
	for(i=0;i<5;i++){
		sumVotes+=helperParty[i];
	}

	double measure=(double)sumVotes/sumSeats;

/*	printf("Election Measure for district:%d with %d seats, is =%f\n",myDistrict->did,myDistrict->seats,measure);
*/
	/* 3 */

	int partySeats[5]={0,0,0,0,0};

	for(i=0;i<5;i++){
		if(helperParty[i]>0 && measure>0){
			partySeats[i]=(int)floor(helperParty[i]/measure);
		}
	}
	
/*	for(i=0;i<5;i++){
		printf("Party:%d has %d seats in district %d\n",Parties[i].pid,partySeats[i],did);
	}
	printf("The district has :%d seats\n",myDistrict->seats);*/
	/* 4 */

	for(i=0; i<5; i++){
        	struct party *party = &Parties[i];
        
        	int seats=partySeats[i];
        	current=myDistrict->candidates;

        	while(current != NULL && seats > 0){
        		if(current->pid == party->pid && current->elected == 0){
                		current->elected=1;
                		seats--;
                		myDistrict->allotted++;

                		/* a new node for the new voted candidate */
                		struct candidate * newCandidate = malloc(sizeof(struct candidate));
                		if(!newCandidate){
                    			fprintf(stderr, "Fail on memory allocation\n");
                   		 	return;
                		}
                		*newCandidate = *current;
				newCandidate->next = NULL;
                		newCandidate->prev = NULL;

                		/* add him on elected list sorted */
                		if(party->elected == NULL || newCandidate->votes > party->elected->votes) {
                    		/* if list is empty or the first one has less votes from the new one */
                    			newCandidate->next = party->elected;
                    			if(party->elected != NULL){
                        			party->elected->prev = newCandidate;
                    			}		
                    			party->elected = newCandidate;
                		}else{
                    			/* in any other case looking for the right position on the list */
                    			struct candidate *current_elected = party->elected;
                    			while(current_elected->next != NULL && current_elected->next->votes >= newCandidate->votes){
                        			current_elected = current_elected->next;
                    			}

                    			/* add the new candidate at list on the right position */
                    			newCandidate->next = current_elected->next;
                    			if(current_elected->next){
                        			current_elected->next->prev = newCandidate;
                    			}
                    			current_elected->next = newCandidate;
                    			newCandidate->prev = current_elected;
                		}

                		/* number of elected candidates at the specific party increament*/
                		party->nelected++;
           		}
			current = current->next;
		}
	}

	
	/* Print */

    	printf("M %d\nSeats = \n",myDistrict->did);
   	current=myDistrict->candidates;
    	while(current != NULL){
            	if(current->elected==1){
			printf("\t<%d> <%d> <%d>\n", current->cid, current->pid, current->votes);
		}
		current = current->next;
    	}
    	printf("DONE\n");
	
	/* check */
/*	int count=0;
	for(i=0;i<5;i++){
		count+=Parties[i].nelected;
	}
	printf("%d\n",count);*/
	return; 
}

/* Event G */

void form_government(void){
	
	struct party * myParty=&Parties[0];
	struct district * myDistrict;
	struct candidate * myCandidate;

	int maxElected=myParty->nelected;
	int i;

	for(i=1;i<5;i++){	/* finding the Party with max elected candidates */
		if(Parties[i].nelected > maxElected){
			myParty=&Parties[i];
			maxElected=myParty->nelected;
		}
	}
	
	printf("G\nSeats=\n");

	for(i=0;i<56;i++){
		
		myDistrict=&Districts[i]; /* taking the first district to share the left over seats */

		int leftSeats=myDistrict->seats-myDistrict->allotted;	/* calculate the left over seats for each district */

		if(leftSeats <= 0) continue;	/* that means district has already share his all district */

		myCandidate=myDistrict->candidates;	/* taking the first element of district candidates list */

		while(myCandidate!=NULL && leftSeats>0){	/* checking if candidate list is over or leftSeats over */
			if(myCandidate->pid==myParty->pid && myCandidate->elected==0){	/* candidate must be from the same party of the first party and must be not elected */
				
				myCandidate->elected=1;	/* candidate is elected now */
				leftSeats--;	/* 1 seat less from left overs */
				myDistrict->allotted++;	/* District alloted seats increament */
				myParty->nelected++;	/* increase the counter of number elected at this party */

				/* printing when found */
				printf("\t<%d> <%d> <%d>\n",myDistrict->did,myCandidate->cid,myCandidate->votes);

				struct candidate * newCandidate=malloc(sizeof(struct candidate));	/* initialize the new elected candidate */
				*newCandidate=*myCandidate;
				newCandidate->next=NULL;
				newCandidate->prev=NULL;
				
				/* the list with elected candidates at struct party is sorted */
				
				if(myParty->elected == NULL || myParty->elected->votes < newCandidate->votes){
                    			newCandidate->next=myParty->elected;	/* if list is null or the new elected candidate has more votes then put him at the head */
                    			if(myParty->elected != NULL){
                        			myParty->elected->prev=newCandidate;
                    			}
                    			myParty->elected=newCandidate;
                		}else{	/* else access the list and find the position that must be */
                    			struct candidate * current=myParty->elected;
                   			while(current->next != NULL && current->next->votes >= newCandidate->votes){
                        			current=current->next;
                    			}
                    			newCandidate->next=current->next;
                    			if (current->next != NULL) {
                        			current->next->prev=newCandidate;
                   			}	/* update the party elected list */
                   			current->next=newCandidate;
                    			newCandidate->prev=current;
                		}	
			}
			myCandidate=myCandidate->next;
		}
		
		myCandidate=myDistrict->candidates;

		/* taking the candidates list of district to add from other parties as elected */
		myCandidate=myDistrict->candidates;
		while(myCandidate!=NULL && leftSeats>0){
			if(myCandidate->elected==0){

				myCandidate->elected=1;	/* candidate is elected now */
				leftSeats--;	/* 1 seat less from left overs */
				myDistrict->allotted++;	/* District alloted seats increament */
				
				/* printing when found */
				printf("\t<%d> <%d> <%d>\n",myDistrict->did,myCandidate->cid,myCandidate->votes);

				for(int j=0;j<5;j++){
					if(myCandidate->pid==Parties[j].pid){	/* find the party with belongs candidate */
						Parties[j].nelected++;	/* increase the counter of number elected at this party */

						struct candidate * newCandidate=malloc(sizeof(struct candidate));
						*newCandidate=*myCandidate;	/* copy the datas */
						newCandidate->next=NULL;
						newCandidate->prev=NULL;

						/* if list is empty of the new elected candidate has more votes than the first put him at the head */
						if(Parties[j].elected==NULL || Parties[j].elected->votes < newCandidate->votes){
							newCandidate->next=Parties[j].elected;
                            				if(Parties[j].elected != NULL){
                                				Parties[j].elected->prev=newCandidate;
                            				}
                            				Parties[j].elected=newCandidate;
						}else{	/* in any other case find the right position of him */
							struct candidate * current=Parties[j].elected;
							while(current->next != NULL && current->next->votes > newCandidate->votes){
                                				current=current->next;
                            				}	
							newCandidate->next=current->next;
							if(current->next != NULL){
								current->next->prev=newCandidate;
							}
							current->next=newCandidate;
							newCandidate->prev=current;
						}
						break;
					}
				}
			}
			myCandidate=myCandidate->next;
		}
	}            	


	/* Print */
/*
	for(i=0;i<countD;i++){
		struct district * current = &Districts[i];
		struct candidate * currentC=current->candidates;

		while(currentC!=NULL){
			if(currentC->elected==1){
				printf("\t<%d> <%d> <%d>\n",current->did,currentC->cid,currentC->votes);
			}
			currentC=currentC->next;
		}
	}*/
	int count=0;
	for(i=0;i<countD;i++){
		struct district * current=&Districts[i];
		struct candidate * candidate=current->candidates;

		while(candidate!=NULL){
			if(candidate->elected==1){
				count++;
			}
			candidate=candidate->next;
		}
	}

	printf("DONE\n");
	return;	
}

/* Event N*/

void form_parliament(void){

	struct candidate * front=NULL; /* header of our final merged list */
    	struct candidate * rear=NULL; 	/* the tail of our final merged list */
    	struct candidate * headers[5];	/* list of the headers for each elected candidates list */
	int i;

	for(i=0;i<5;i++){	/* taking each header */
		headers[i]=Parties[i].elected;
	}

	while(1){
	
		int maxVotes=-1;
		int index=-1;
		
		for(i=0;i<5;i++){	 //looking who candidate has the most votes 
			if(headers[i]!=NULL){
				if(maxVotes<headers[i]->votes){
					maxVotes=headers[i]->votes;
					index=i;
				}
			}
		}
		if(index==-1) break;	//if headers are null or something went wrong 

		struct candidate * newMember=malloc(sizeof(struct candidate));

		*newMember=*headers[index];
		newMember->next=NULL;

		if(front==NULL){	//if list is empty 
            		front=newMember;
            		rear=newMember;
        	}else{
            		rear->next=newMember;
            		rear=newMember;
        	}

		headers[index]=headers[index]->next;	
	}

	/* second one (not O(n))*/
	
/*	for(i=0; i<5; i++){
        	struct candidate * currentCandidate=Parties[i].elected;

        	while(currentCandidate != NULL){
            		struct candidate * newNode=malloc(sizeof(struct candidate));
            		*newNode=*currentCandidate;
            		newNode->next=NULL;

            		if(front == NULL || front->votes < newNode->votes){
                		newNode->next=front;
                		front=newNode;
                		if(rear==NULL){
                    			rear=newNode;
				}
            		}else{
                		struct candidate * curr=front;
                		while(curr->next != NULL && curr->next->votes >= newNode->votes){
                 	  		curr=curr->next;
                		}
                		newNode->next=curr->next;
                		curr->next=newNode;
                		if(newNode->next == NULL){
                    			rear=newNode;
                		}
            		}
            		currentCandidate=currentCandidate->next;
        	}
    	}*/


    	/* final parliament list*/
    	Parliament.members=front;

    	/* Print */
    	printf("N\nMembers =\n");
    	int idx=0;
    	struct candidate * curr=front;
    	while(curr != NULL){
        	printf("\t<%d> <%d> <%d>\n",curr->cid,curr->pid,curr->votes);
        	curr=curr->next;
        	idx++;
    	}
    	printf("DONE:(%d)\n",idx);
}

/* Event I */

void print_district(int did){

	struct district * myDistrict=NULL;
	int i;

	for(i=0;i<countD;i++){
        	if(Districts[i].did==did){
            		myDistrict=&Districts[i];
            		break;
        	}
    	}

    	if(myDistrict==NULL){
        	printf("District with did: %d , does not exist.\n",did);
        	return;
    	}

    	printf("I %d\n",did);
    	printf("Seats = <%d>\n",myDistrict->seats);
    	printf("Blanks = <%d>\n",myDistrict->blanks);
    	printf("Voids = <%d>\n",myDistrict->voids);
    	printf("Candidates = \n");

    	struct candidate * myCandidate=myDistrict->candidates;
    	while(myCandidate != NULL){
        	printf("\t<%d> <%d> <%d>\n",myCandidate->cid,myCandidate->pid,myCandidate->votes);
        	myCandidate=myCandidate->next;
    	}

    	printf("Stations = ");
    	struct station * myStation = myDistrict->stations;
    	while(myStation != NULL){
        	printf("<%d>",myStation->sid);
        	if(myStation->next != NULL) printf(", ");
       		myStation=myStation->next;
    	}
    	printf("\nDONE\n");
}

/* Event J */

void print_station(int sid, int did){
	struct district * myDistrict=NULL;
    	struct station * myStation=NULL;
	int i;

    	for(i=0;i<countD;i++){
        	if(Districts[i].did==did){
            		myDistrict=&Districts[i];
            		break;
        	}
    	}

    	if(myDistrict==NULL){
        	printf("District with did:%d , does not exist.\n",did);
        	return;
    	}

    	myStation=myDistrict->stations;
    	while(myStation != NULL){
       		if(myStation->sid==sid){	
            		break;
        	}
        	myStation=myStation->next;
    	}

    	if(myStation==NULL){
        	printf("Station with sid:%d , does not exist in District %d.\n", sid, did);
        	return;
    	}

    	printf("J <%d>\n",sid);
    	printf("Registered = <%d>\n",myStation->registered);
    	printf("Voters = \n");

    	struct voter * myVoter=myStation->voters;
    	while(myVoter != myStation->vsentinel){
        	printf("\t<%d> <%d>\n",myVoter->vid,myVoter->voted);
        	myVoter=myVoter->next;
    	}

    	printf("DONE\n");
}

/* Event K */

void print_party(int pid){
	struct party * myParty=NULL;
	int i;

    	for(i=0;i<countP;i++){
        	if(Parties[i].pid == pid){
            		myParty=&Parties[i];
            		break;
        	}
    	}

    	if(myParty == NULL){
        	printf("Party with pid:%d , does not exist.\n",pid);
        	return;
    	}

    	printf("K <%d>\n",pid);
    	printf("Elected=\n");

    	struct candidate * myCandidate=myParty->elected;
    	while(myCandidate != NULL){
        	printf("\t<%d> <%d>\n",myCandidate->cid,myCandidate->votes);
        	myCandidate=myCandidate->next;
    	}

    	printf("DONE\n");
}

/* Event L */

void print_parliament(void){
	printf("L\nMembers =\n");

    	struct candidate * myCandidate=Parliament.members;
    	while(myCandidate != NULL){
        	printf("\t<%d> <%d> <%d>\n",myCandidate->cid,myCandidate->pid,myCandidate->votes);
        	myCandidate=myCandidate->next;
    	}

    	printf("DONE\n");
}

/* BONUS EVENT */

void destroy_structures(void){

	int i;

	for(i=0;i<countD;i++){
		struct station * myStation=Districts[i].stations;

		while(myStation!=NULL){

			struct voter * myVoter=myStation->voters;
			while(myVoter!=myStation->vsentinel){
				
				struct voter * temp=myVoter;
				myVoter=myVoter->next;
				free(temp);
			}
			free(myStation->vsentinel);
			
			struct station * temp2=myStation;
			myStation=myStation->next;
			free(temp2);
		}

		struct candidate * myCandidate=Districts[i].candidates;
		while(myCandidate!=NULL){
			struct candidate * temp3=myCandidate;
			myCandidate=myCandidate->next;
			free(temp3);
		}
	}

	for(i=0;i<countP;i++){
		struct candidate * myCandidate=Parties[i].elected;
		while(myCandidate!=NULL){
			struct candidate * temp=myCandidate;
			myCandidate=myCandidate->next;
			free(temp);
		}
	}

	struct candidate * current1=Parliament.members;
    	while(current1 != NULL){
        	struct candidate * temp3=current1;
        	current1=current1->next;
        	free(temp3);
    	}

	for (i = 0; i < 56; i++) {
        	Districts[i].stations = NULL;
        	Districts[i].candidates = NULL;
    	}

    	for(i=0;i<5;i++){
        	Parties[i].elected = NULL;
    	}

    	Parliament.members = NULL;

	printf("\n************ All structures have been destroyed ************\n\n");
}

int main(int argc, char *argv[])
{
	FILE *event_file;
	char line_buffer[MAX_LINE];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	event_file = fopen(argv[1], "r");
	if (!event_file) {
		perror("fopen error for event file open");
		exit(EXIT_FAILURE);
	}

	while (fgets(line_buffer, MAX_LINE, event_file)) {
		char *trimmed_line;
		char event;
		int did, sid, vid, cid, pid, seats;
		int ret = 0;
		/*
		 * First trim any whitespace
		 * leading the line.
		 */
		trimmed_line = line_buffer;
		while (trimmed_line && isspace(*trimmed_line))
			trimmed_line++;
		if (!trimmed_line)
			continue;
		/* 
		 * Find the event,
		 * or comment starting with #
		 */
		if (sscanf(trimmed_line, "%c", &event) != 1) {
			fprintf(stderr, "Could not parse event type out of input line:\n\t%s",
					trimmed_line);
			fclose(event_file);
			exit(EXIT_FAILURE);
		}

		switch (event) {
			/* Comment, ignore this line */
			case '#':
				break;
			case 'A':
				announce_elections();
				break;
			case 'D':
				if (sscanf(trimmed_line, "D %d %d", &did, &seats) != 2) {
					fprintf(stderr, "Event D parsing error\n");
					ret = 1;
					break;
				}
				ret = create_district(did, seats);
				break;
			case 'S':
				if (sscanf(trimmed_line, "S %d %d", &sid, &did) != 2) {
					fprintf(stderr, "Event S parsing error\n");
					ret = 1;
					break;
				}
				ret = create_station(sid, did);
				break;
			case 'P':
				if (sscanf(trimmed_line, "P %d", &pid) != 1) {
					fprintf(stderr, "Event P parsing error\n");
					ret = 1;
					break;
				}
				create_party(pid);
				break;
			case 'C':
				if (sscanf(trimmed_line, "C %d %d %d", &cid, &did, &pid) != 3) {
					fprintf(stderr, "Event C parsing error\n");
					ret = 1;
					break;
				}
				ret = register_candidate(cid, did, pid);
				break;
			case 'R':
				if (sscanf(trimmed_line, "R %d %d %d", &vid, &did, &sid) != 3) {
					fprintf(stderr, "Event R parsing error\n");
					ret = 1;
					break;
				}
				ret = register_voter(vid, did, sid);
				break;
			case 'U':
				if (sscanf(trimmed_line, "U %d", &vid) != 1) {
					fprintf(stderr, "Event U parsing error\n");
					ret = 1;
					break;
				}
				ret = unregister_voter(vid);
				break;
			case 'E':
				delete_empty_stations();
				break;
			case 'V':
				if (sscanf(trimmed_line, "V %d %d %d", &vid, &sid, &cid) != 3) {
					fprintf(stderr, "Event V parsing error\n");
					ret = 1;
					break;
				}
				ret = vote(vid, sid, cid);
				break;
			case 'M':
				if (sscanf(trimmed_line, "M %d", &did) != 1) {
					fprintf(stderr, "Event M parsing error\n");
					ret = 1;
					break;
				}
				count_votes(did);
				break;
			case 'G':
				form_government();
				break;
			case 'N':
				form_parliament();
				break;
			case 'I':
				if (sscanf(trimmed_line, "I %d", &did) != 1) {
					fprintf(stderr, "Event I parsing error\n");
					ret = 1;
					break;
				}
				print_district(did);
				break;
			case 'J':
				if (sscanf(trimmed_line, "J %d %d", &sid, &did) != 2) {
					fprintf(stderr, "Event J parsing error\n");
					ret = 1;
					break;
				}
				print_station(sid, did);
				break;
			case 'K':
				if (sscanf(trimmed_line, "K %d", &pid) != 1) {
					fprintf(stderr, "Event I parsing error\n");
					ret = 1;
					break;
				}
				print_party(pid);
				break;
			case 'L':
				print_parliament();
				break;
			default:
				fprintf(stderr, "WARNING: Unrecognized event %c. Continuing...\n",
						event);
				ret = 1;
				break;
		}
		fprintf(stderr, "Event %c %s.\n", event,
				ret ? "failed" : "succeeded");
		
	}
	fclose(event_file);
	destroy_structures();
	return 0;
}
