#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/
void main(int argc,char **argv)
{
FILE *fisc,*fvec,*ffau,*fres,*fbench,*fout;             //file pointers used for .isc file, .vec file, .faults file and resultfile
int Max,Opt,Npi,Npo,Total,Tfs,Max2,or_gate_id;              //maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults

NODE graph[Mnod];                         //structure used to store the ckt information in .isc file 
NODE node[Mnod];
//PATTERN vector[Mpt];                      //structure used to store the input vectors information in .vec file 

//FAULT stuck[Mft];                      //structure used to store the faults information in .faults file
int a,b,c,d;                             //random variables



//Read the .isc file and store the information in graph structure
fisc=fopen(argv[1],"r");                           //file pointer to open .isc file 
// fisc = fopen("c17.isc", "r");
Max=0; 
Max=ReadIsc(fisc,graph);                 //read .isc file and return index of last node in graph formed
fclose(fisc);                            //close file pointer for .isc file
// PrintCircuit(graph,Max);                 //print all members of graph structure

fbench = fopen(argv[2],"r");
// fbench = fopen("c17.bench", "r");
Max2 = 0;
Max2 =  Readbench( fbench, node);
fclose(fbench);

// fbench = fopen(argv[3], "r");

duplicateGate( node, Max2); 
PrintCircuit(node,3*Max2);

fout = fopen("output.bench", "w");  // Open file for writing
//fout = fopen(argv[4], "w");
WriteBenchFile(fout, graph, or_gate_id, Max2);

fclose(fout);
//Read the .vec file and store the information in  vector structure




 

//Opt=0; 
//Opt=atoi(argv[3]);                          //getting the option from terminal for xval
//fres=fopen(argv[2],"w");                           //file pointer to open .out file for printing results

//fclose(fout);
//Perform Logic Simulation for each Input vector and print the Pos .val in output file   

//fclose(fres);                                                  //close file pointer for .out file

ClearCircuit(graph,Mnod);                                      //clear memeory for all members of graph
//for(a=0;a<Total;a++){ bzero(vector[a].piv,Mpi); }                //clear memeory for all members of vector
return;
}//end of main
/****************************************************************************************************************************/

