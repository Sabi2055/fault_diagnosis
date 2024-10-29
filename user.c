#include "graph.h"

int Readbench(FILE *fbench, NODE *graph)
{
    char line[Mlin];
    int i, node_count, values;
    //int last_output1 = -1, last_output2 = -1;

    // Initialize all nodes in the graph structure
    for (i = 0; i < Mnod; i++) {      InitializeCircuit(graph, i);   }

    // Skip the comment lines
    fgets(line, Mlin, fbench);
    while (line[0] == '#') {
        fgets(line, Mlin, fbench);
    }

    // Read line by line
    while (fgets(line, Mlin, fbench) != NULL) {
        // printf("%s", line);
        
        // Handle "INPUT" type
        if (strncmp(line, "INPUT", 5) == 0) {
            sscanf(line, "INPUT (%d)", &node_count);
            graph[node_count].Type = INPT;
        }
        // Handle "OUTPUT" type
        else if (strncmp(line, "OUTPUT", 6) == 0) {
            sscanf(line, "OUTPUT (%d)", &node_count);
            //graph[node_count].Type = NAND;
            graph[node_count].Po = 1;
        }
		else {
            // printf("%s",line);

			if (strstr(line, "NAND") != NULL) {
                sscanf(line, "%d = NAND", &node_count);
                graph[node_count].Type = NAND;  // Set the gate type
        	}
        	else if (strstr(line, "AND") != NULL) {
                sscanf(line, "%d = AND", &node_count);
                graph[node_count].Type = AND;
        	}
        	else if (strstr(line, "OR") != NULL) {
                sscanf(line, "%d = OR", &node_count);
                graph[node_count].Type = OR;
        	}
        	else if (strstr(line, "NOR") != NULL) {
                sscanf(line, "%d = NOR", &node_count);
                graph[node_count].Type = NOR;
        	}
        	else if (strstr(line, "XOR") != NULL) {
                sscanf(line, "%d = XOR", &node_count);
                graph[node_count].Type = XOR;
        	}
        	else if (strstr(line, "XNOR") != NULL) {
                sscanf(line, "%d = XNOR", &node_count);
                graph[node_count].Type = XNOR;
        	}
        	else if (strstr(line, "BUFF") != NULL) {
                sscanf(line, "%d = BUFF", &node_count);
                graph[node_count].Type = BUFF;
        	}
        	else if (strstr(line, "NOT") != NULL) {
                sscanf(line, "%d = NOT", &node_count);
                graph[node_count].Type = NOT;
        	}
			else{
                continue;
			}
            // Handle other node types
		    strstr(line, "%d") !=NULL;
			sscanf(line, "%d", &node_count);
			
            // Read node number
            //sscanf(line, "%d", &node_count);
            //graph[node_count].Type = NAND;

            // Extract the list of values between parentheses
            char *start = strchr(line, '(');
            char *end = strchr(line, ')');
            if (start != NULL && end != NULL) {
                start++;  // Move past the opening '('
                int i = 0;

                // Read all integers between the parentheses
                while (start < end && sscanf(start, "%d", &values) == 1) {
                    
                    InsertList(&graph[node_count].Fin, values);
                    graph[node_count].Nfi++;
                    InsertList(&graph[values].Fot, node_count);
                    graph[values].Nfo++;
                    start = strchr(start, ',');
                    if (start != NULL && start < end) {
                        start++;  // Move past the comma
                    } else 
					{
                        break;  // No more commas
                    }
                }
            }
	    }  
    }
    return node_count;
}  


// To duplicate the circuit
void duplicateGate( NODE *graph, int Max2)  {
    int i,node_count = -1, values;
    char line[Mlin];
    int last_output1 = -1, last_output2 = -1;
    int xnor_output_count = 0; 

    LIST *temp;
    for ( i=0; i<=Max2;i++){
        if ( graph[i].Type == 0) continue;
        if (graph[i].Type == INPT) continue;
        graph[i+Max2].Type = graph[i].Type;
        temp = graph[i].Fin;
        while (temp != NULL){
            
            if (graph[temp->id].Type== INPT ) {

                InsertList(&graph[i + Max2].Fin, temp->id);
                InsertList(&graph[temp->id].Fot, i+Max2);
                graph[i+Max2].Nfi++;
                graph[temp->id].Nfo++;

            }
            else {
                InsertList(&(graph[i + Max2].Fin), temp->id + Max2);
                graph[i+Max2].Nfi++;
                InsertList(&(graph[temp->id + Max2].Fot), i + Max2);
                graph[temp->id + Max2].Nfo++;
            }
            if (graph [i].Po ==1) {
                graph[i+Max2].Po =1;
            }

            
            
            
            printf("%d ", temp->id);
            temp = temp->next;
        }
        printf("\n");
        temp = graph[i+Max2].Fin;
    }
    //last_output1 = last_output2 = -1;
    //int node_count=-1;
    int op_count = 0;
    for(i=0; i<Max2; i++){
        if (graph[i + Max2].Po ==1){
            last_output2 = last_output1;
            last_output1 = i+Max2;
            op_count++;
        }
    }
    
    int or_gate_id = Max2*2 +op_count+1;
    graph[or_gate_id].Type = OR;
    graph[or_gate_id].Nfi = 0;   // To Initialize number of fan-ins
    graph[or_gate_id].Nfo = 0;
    if (last_output1 != -1 && last_output2 != -1) {
            int i=1;
            //int or_gate_id = Max2 + Max2 + i;  // OR gate will come after all XNORs
            int xnor_output_count = 0;

            // Create XNOR gates for every pair of outputs( one is original and another is replicated one)
            for (i = 0; i <=Max2; i++) {
                if (graph[i].Po == 1) {
                    int replica_output = i + Max2;
                    int xnor_gate_id = Max2 + Max2 + 1 + xnor_output_count;

                    // Add XNOR gate
                    graph[xnor_gate_id].Type = XNOR;
                    graph[xnor_gate_id].Nfi = 0;   // Initialize number of fan-ins
                    graph[xnor_gate_id].Nfo = 0;
                    InsertList(&graph[xnor_gate_id].Fin, i);          // Original output
                    InsertList(&graph[xnor_gate_id].Fin, replica_output);  // Replicated output
                    graph[xnor_gate_id].Nfi += 2;

                    InsertList(&graph[or_gate_id].Fin, xnor_gate_id);  // Connect XNOR output to OR gate
                    graph[or_gate_id].Nfi++;  // Increment fan-in for the OR gate
                    InsertList(&graph[xnor_gate_id].Fot, xnor_gate_id);  // XNOR gate output connects to OR gate
                    graph[xnor_gate_id].Nfo++; 
                    graph[or_gate_id].Po=1;

                    // Connect XNOR gate outputs
                    InsertList(&graph[i].Fot, xnor_gate_id);
                    graph[i].Nfo++;
                    //InsertList(&graph[replica_output].Fot, xnor_gate_id);
                    graph[replica_output].Nfo++;

                    xnor_output_count++;  // Increment for each XNOR gate
                }
            }

    } else {
        printf("Error: Not enough output nodes found to create XNOR gates.\n");
    }
}

void WriteBenchFile(FILE *fout, NODE *graph, int or_gate_id, int Max2){
    int i;
    LIST *temp;
    if (fout == NULL) {
        printf("Failed to open file");
        return; // Exit if the file pointer is NULL
    }
    fprintf(fout, "# \n");
    fprintf(fout, "# \n");
    fprintf(fout, "# \n");
    fprintf(fout, "# \n\n");
    for(i=0;i<Max2;i++){
        if(graph[i].Type==INPT){
            fprintf(fout,"INPUT(%d)\n",i);
        }
    }
    
    fprintf(fout,"OUTPUT(%d)\n",or_gate_id);
    for (i=0;i<=or_gate_id;i++){
        temp=graph[i].Fin;
        while (temp !=NULL)
        {
            fprintf(fout, "%d=",temp->id);
            switch (graph[temp->id].Type)
            {
            case NAND:
                fprintf(fout,"NAND(");
                break;
            case AND:
                fprintf(fout,"AND(");
                break;
            case OR:
                fprintf(fout,"OR(");
                break;
            case XOR:
                fprintf(fout,"XOR(");
                break;
            case BUFF:
                fprintf(fout,"BUFF(");
                break; 
            case NOR:
                fprintf(fout,"NOR(");
                break; 
            case NOT:
                fprintf(fout,"NOT(");
                break;              
            //default:
                //break;
            }
        temp=temp->next  ;  
        }
        
    }
    

}




