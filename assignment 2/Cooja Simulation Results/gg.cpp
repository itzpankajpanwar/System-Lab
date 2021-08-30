
  
// command to run:
// ./routing -top test.txt -conn NSFNET_100.doc -rt routingfile.txt -ft forwarding.txt -path pathfile.txt -flag hop -p 0



#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string.h>						// for strcat() and strcpy()
#include <limits.h>						// for INT_MAX
#include <algorithm>					// for min()
#include <stdio.h>

#define max_nodes 50
#define max_connections 200
#define conn_params 5

using namespace std;

long double my_min(long double x, long double y)
{
	if(x <= y)
		return x;
	else 
		return y;
}

bool read_topology(char filename[256], int* N, int* E, long double topology[max_nodes][max_nodes][2])
{
    FILE *reader;
    int i, j, k, source, dest;
    long double prop_delay, capacity, reliability;

    reader = fopen(filename,"r");
    
    if(!reader)
    {
        printf("\nError: Topology File does not exist");
        return false;
    }

    else
    {
        fscanf(reader,"%d %d",N,E);				// first line gives number of nodes and edges

        // read topology
        for(i = 0; i < *E; i++)
        {
            fscanf(reader,"%d %d %Lf %Lf %Lf",&source,&dest,&prop_delay,&capacity, &reliability);
            topology[source][dest][0] = topology[dest][source][0] = prop_delay;
            topology[source][dest][1] = topology[dest][source][1] = capacity;
        }

        fclose(reader);

        // printing the read topology
        // for(k = 0; k < *N; k++)
        // {
        //     for(j = 0; j < *N; j++)
        //     {
        //         printf("%d %d\t",topology[k][j][0],topology[k][j][1]);
        //     }
        //     printf("\n");
        // }
        return true;
    }
}

bool read_connections(char filename[256], int* num_connections, int connections[max_connections][conn_params])
{
    FILE *reader;
    int i, src, dest, bi_min, bi_ave, bi_max;

    reader = fopen(filename,"r");
    
    if(!reader)
    {
        printf("\nError: Connection File does not exist");
        return false;
    }

    else
    {
        fscanf(reader,"%d", num_connections);				// first line gives number of connections

        // read connections
        for(i = 0; i < *num_connections; i++)
        {
            fscanf(reader,"%d %d %d %d %d",&src,&dest,&bi_min,&bi_ave, &bi_max);
            connections[i][0] = src;
            connections[i][1] = dest;
            connections[i][2] = bi_min;
            connections[i][3] = bi_ave;
            connections[i][4] = bi_max;
           
        }

        fclose(reader);

        // printing the read connections
        // for(int k = 0; k < *num_connections; k++)
        // {
        //     for(int j = 0; j < conn_params; j++)
        //     {
        //         printf("%d ",connections[k][j]);
        //     }
        //     printf("\n");
        // }
        return true;
    }
}

void all_pairs_shortest_path(int cost_path[][max_nodes], int parent_path[][max_nodes], int num_nodes)
{
  
    int i, j, k;  
    for (k = 0; k < num_nodes; k++)  
    {  
        // Pick all vertices as source one by one  
        for (i = 0; i < num_nodes; i++)  
        {  
            // Pick all vertices as destination for the above picked source  
            for (j = 0; j < num_nodes; j++)  
            {  
                // If vertex k is on the shortest path from i to j, then update the value of dist[i][j]  
                if (cost_path[i][k]!=INT_MAX && cost_path[k][j]!=INT_MAX && cost_path[i][k] + cost_path[k][j] < cost_path[i][j])
                {  
                    cost_path[i][j] = cost_path[i][k] + cost_path[k][j];
                    parent_path[i][j] = parent_path[k][j];
                }
            }  
        }  
    }

}

void store_shortest_paths(int parent_path1[][max_nodes], int num_nodes, const char rank[], int shortest_path[][max_nodes][max_nodes])
{
	FILE *writer;
	char filename[256] = "";
	int i, j, prev, pathnode;

	strcat(filename, "shortest_path_");
	strcat(filename, rank);
	strcat(filename, ".txt");

	writer = fopen(filename, "w");

	if(writer == NULL)
		cout<< "\nError: Can't open file";

	for(i = 0; i < num_nodes; i++)
	{
		for(j = 0; j < num_nodes; j++)
		{
			pathnode = 0;

			if(i == j)
				shortest_path[j][i][pathnode] = -3;		// -3 indicates self-loop - no path
				
			else
			{
				prev = parent_path1[i][j];

				fprintf(writer, "%d %d ", j, i);
				// 	// floyd-warshall stores path in reverse order; so here we make j as source,
				
												// index to store the path node
				while(prev != -1)		// reaversing till we reach source
				{
					fprintf(writer, "%d ", prev);
					shortest_path[j][i][pathnode] = prev;
					pathnode++;
					prev = parent_path1[i][prev];
				}
				if(i != j)
					fprintf(writer, "\n");
			}	
			
			
			
		}
	}

	fclose(writer);
}

void original_topology(long double topology[][max_nodes][2], int cost_path[][max_nodes], int parent_path[][max_nodes], int num_nodes, char hop_or_dist[])
{
	int i, j;
	for(i = 0; i < num_nodes; i++)
	{
		for(j = 0; j < num_nodes; j++)
		{
			if(i == j)			// loop
			{
				// no loop exists
				cost_path[i][j] = 0;
				parent_path[i][j] = -1;
			}

			else if(topology[i][j][0] != 0)		// delay exists means link exists 
			{
				if((strcmp(hop_or_dist, "hop")) == 0)
					cost_path[i][j] = 1;
				else
					cost_path[i][j] = topology[i][j][0] * 10;

				parent_path[i][j] = i;
		
			}
			
			else				// no path exists, so make link weight infinity
			{
				cost_path[i][j] = INT_MAX;
				parent_path[i][j] = -1;

			}
		}
	}

}

void remove_shortest_path(int shortest1[][max_nodes][max_nodes], int cost_path[][max_nodes], int parent_path[][max_nodes], int src, int dest)
{
	int pathlength, iter;
	pathlength = cost_path[src][dest];		// should be replaced by hop matrix

	iter = 0;

	int i, j;
	i = src;
	j = shortest1[src][dest][iter];
	while(j != -1)
	{
		cost_path[i][j] = INT_MAX;
		cost_path[j][i] = INT_MAX;
		parent_path[i][j] = -1;
		parent_path[j][i] = -1;

		iter++;
		i = j;
		j = shortest1[src][dest][iter];
	}
	
}

void store_second_best_path(int parent_path[][max_nodes], int shortest_path[][max_nodes][max_nodes], int i, int j)
{
	// i = src, j = dest
	int prev, pathnode;
	FILE *writer;
	writer = fopen("shortest_path_2.txt", "a");

	pathnode = 0;

	if(i == j)
		shortest_path[j][i][pathnode] = -3;		// -3 indicates self-loop - no path

		
	// floyd-warshall stores path in reverse order; so here we make j as source,
	// for every destination i
	prev = parent_path[i][j];
	fprintf(writer, "%d %d ", j, i);


								// index to store the path node
	while(prev != -1)		// reaversing till we reach source
	{
		fprintf(writer, "%d ", prev);
		shortest_path[j][i][pathnode] = prev;
		pathnode++;
		prev = parent_path[i][prev];
	}

	if(i != j)
		fprintf(writer, "\n");

	fclose(writer);	
	
	
}

void print_shortest_paths(int shortest_path[][max_nodes][max_nodes], int num_nodes)
{
	int i, j, k;
	for(i = 0; i <num_nodes; i++)
	{
		cout<<endl;
		for(j = 0; j < num_nodes ; j++)
		{
			k = 0;
			cout<<i<<"to"<<j<<": ";
			while(shortest_path[i][j][k] != -1)
			{
				cout<<shortest_path[i][j][k]<<" ";
				k++;
			}
			cout<<endl;
		}
	}
}

void print_cost_and_parent(int cost_path2[][max_nodes], int parent_path2[][max_nodes], int src, int dest, int num_nodes)
{
	int i, j;
	cout<<"\nCost:\n";
	for(i = 0; i<num_nodes; i++)
	{
		cout<<endl;
		for(j = 0; j<num_nodes; j++)
			cout<<cost_path2[i][j]<<" "; 
	}


	cout<<"\nParent:\n";
	for(i = 0; i<num_nodes; i++)
	{
		cout<<endl;
		for(j = 0; j<num_nodes; j++)
			cout<<parent_path2[i][j]<<" "; 
	}
}

bool check_shortest_path(int* pathlength, int shortest_path[][max_nodes][max_nodes], int src, int dest, long double topology[][max_nodes][2], long double bi)
{
	int iter, prev, curr;

	iter = 0;
	prev = src;
	curr = shortest_path[src][dest][iter];
	while(prev != dest)
	{
		if(topology[prev][curr][1] < bi)
			return false;
		iter++;
		prev = curr;
		curr = shortest_path[src][dest][iter];
	}

	// confirmed that this connection can be established using current shortest path
	// now reducing capacity of each link in this shortest path by bi_equiv (channel given)
	*pathlength = 0;
	prev = src;
	curr = shortest_path[src][dest][(*pathlength)];
	while(prev != dest)
	{
		topology[prev][curr][1] -= bi;
		topology[curr][prev][1] -= bi;
		*pathlength = *pathlength + 1;
		prev = curr;
		curr = shortest_path[src][dest][(*pathlength)];	 
	}


	return true;

}

void connection_establishment(int p, int num_nodes, char pathfile[], char fw_table_file[], int forwarding_table[max_nodes][max_connections][conn_params], int connections[][conn_params], int num_connections, int shortest_path1[][max_nodes][max_nodes], int shortest_path2[][max_nodes][max_nodes], long double topology[][max_nodes][2])
{

	long double bi_equiv, bi_res;
	bool adequate_BW_SP1;
	int vcid = 0;
	int src, dest, prev, curr, free_slot, pathlength, selected_SP = 1, iter = 0;
	int j, k, l, last_vcid, admitted_count = 0;

	FILE *pathwriter, *output, *fw;

	pathwriter = fopen(pathfile, "w");
	output = fopen("output_file.txt", "w");
	fw = fopen(fw_table_file, "w");


	for(int i = 0; i < num_connections; i++)
	{
		pathlength = -1;
		
		if(p == 0)						// optimistic approach
		{
			bi_equiv = my_min(connections[i][4], (connections[i][3] + 0.25 * (connections[i][4] - connections[i][2])));
			bi_res = bi_equiv;
		}

		else 							// pessimistic approach
			bi_res = connections[i][4];

		adequate_BW_SP1 = check_shortest_path(&pathlength, shortest_path1, connections[i][0], connections[i][1], topology, bi_res);
		
		if(adequate_BW_SP1 == false)
		{
			selected_SP = 2;
			pathlength = -1;
			adequate_BW_SP1 = check_shortest_path(&pathlength, shortest_path2, connections[i][0], connections[i][1], topology, bi_res);
		}

		if(adequate_BW_SP1 == false)
		{
			continue;
			//cout<<"\nConnection from "<<connections[i][0]<< " to "<<connections[i][1]<<" cannot be established !\n";
		}
		
		else
		{
			admitted_count++; 	// connection admitted
			// run a while loop to get the shortest path nodes
			if(selected_SP == 1)
			{
				last_vcid = vcid;
				vcid = vcid + pathlength - 1;
				src = connections[i][0];
				dest = connections[i][1];
				// find first free entry in forwarding table for current source node to 
				// store forawrding info. for current connection
				

				iter = 0;
				prev = src;
				curr = shortest_path1[src][dest][iter];
				while(iter <= pathlength)
				{
					free_slot = 0;
					while(forwarding_table[prev][free_slot][0] != -1)
						free_slot++;

					if(iter == 0)				// for src node, no incoming port
					{
						forwarding_table[prev][free_slot][0] = -2;	// stores id of incoming port
						forwarding_table[prev][free_slot][1] = -2;	// stores vcid of incoming port
						forwarding_table[prev][free_slot][2] = shortest_path1[src][dest][iter];	// stores id of outgoing port
						forwarding_table[prev][free_slot][3] = vcid;	// stores vcid of outgoing port
					}

					else if(iter == 1)
					{
						forwarding_table[prev][free_slot][0] = src;	// stores id of incoming port
						forwarding_table[prev][free_slot][1] = vcid;	// stores vcid of incoming port
						forwarding_table[prev][free_slot][2] = shortest_path1[src][dest][iter];	// stores id of outgoing port
						forwarding_table[prev][free_slot][3] = vcid;	// stores vcid of outgoing port
					}

					else if(iter == pathlength)
					{
						forwarding_table[prev][free_slot][0] = shortest_path1[src][dest][iter-2];	// stores id of incoming port
						forwarding_table[prev][free_slot][1] = vcid;	// stores vcid of incoming port
						forwarding_table[prev][free_slot][2] = -2;	// stores id of outgoing port
						forwarding_table[prev][free_slot][3] = -2;	// stores vcid of outgoing port	
					}
					else
					{
						forwarding_table[prev][free_slot][0] = shortest_path1[src][dest][iter-2];	// stores id of incoming port
						forwarding_table[prev][free_slot][1] = vcid;	// stores vcid of incoming port
						forwarding_table[prev][free_slot][2] = shortest_path1[src][dest][iter];;	// stores id of outgoing port
						forwarding_table[prev][free_slot][3] = vcid;	// stores vcid of outgoing port	
					}

					iter++;
					prev = curr;
					curr = shortest_path1[src][dest][iter];	 
				}


				// writing output file
				// [conn_id, src, dest, path, VCID list, cost]
				fprintf(output, "%d\t%d\t%d\t%d", i, src, dest, src);
				k = 0;
				while(shortest_path1[src][dest][k] != -1)
				{
					fprintf(output, "-%d", shortest_path1[src][dest][k]);
					k++;
				}

				fprintf(output, "\t\t\t{");

				while(last_vcid <= vcid)
				{
					if(last_vcid == vcid)
						fprintf(output, "%d", last_vcid);
					else
						fprintf(output, "%d, ", last_vcid);

					last_vcid++;
				}

				fprintf(output, "}\t%d\n", pathlength);		// assuming hop count as distance metric

				// when current connection is served, then for the next connection,vcid 
				// should start from the next vcid value in line
				vcid++;
			}

			else

			{
				last_vcid = vcid;
				vcid = vcid + pathlength - 1;
				src = connections[i][0];
				dest = connections[i][1];
				// find first free entry in forwarding table for current source node to 
				// store forawrding info. for current connection
				

				iter = 0;
				prev = src;
				curr = shortest_path2[src][dest][iter];
				while(iter <= pathlength)
				{
					free_slot = 0;
					while(forwarding_table[prev][free_slot][0] != -1)
						free_slot++;

					if(iter == 0)				// for src node, no incoming port
					{
						forwarding_table[prev][free_slot][0] = -2;	// stores id of incoming port
						forwarding_table[prev][free_slot][1] = -2;	// stores vcid of incoming port
						forwarding_table[prev][free_slot][2] = shortest_path2[src][dest][iter];	// stores id of outgoing port
						forwarding_table[prev][free_slot][3] = vcid;	// stores vcid of outgoing port
					}

					else if(iter == 1)
					{
						forwarding_table[prev][free_slot][0] = src;	// stores id of incoming port
						forwarding_table[prev][free_slot][1] = vcid;	// stores vcid of incoming port
						forwarding_table[prev][free_slot][2] = shortest_path2[src][dest][iter];	// stores id of outgoing port
						forwarding_table[prev][free_slot][3] = vcid;	// stores vcid of outgoing port
					}

					else if(iter == pathlength)
					{
						forwarding_table[prev][free_slot][0] = shortest_path2[src][dest][iter-2];	// stores id of incoming port
						forwarding_table[prev][free_slot][1] = vcid;	// stores vcid of incoming port
						forwarding_table[prev][free_slot][2] = -2;	// stores id of outgoing port
						forwarding_table[prev][free_slot][3] = -2;	// stores vcid of outgoing port	
					}
					else
					{
						forwarding_table[prev][free_slot][0] = shortest_path2[src][dest][iter-2];	// stores id of incoming port
						forwarding_table[prev][free_slot][1] = vcid;	// stores vcid of incoming port
						forwarding_table[prev][free_slot][2] = shortest_path2[src][dest][iter];;	// stores id of outgoing port
						forwarding_table[prev][free_slot][3] = vcid;	// stores vcid of outgoing port	
					}

					iter++;
					prev = curr;
					curr = shortest_path1[src][dest][iter];	 
				}


				// writing output file
				// [conn_id, src, dest, path, VCID list, cost]
				fprintf(output, "%d\t%d\t%d\t%d", i, src, dest, src);
				k = 0;
				while(shortest_path1[src][dest][k] != -1)
				{
					fprintf(output, "-%d", shortest_path1[src][dest][k]);
					k++;
				}

				fprintf(output, "\t\t\t{");

				while(last_vcid <= vcid)
				{
					if(last_vcid == vcid)
						fprintf(output, "%d", last_vcid);
					else
						fprintf(output, "%d, ", last_vcid);
					last_vcid++;
				}

				fprintf(output, "}\t%d\n", pathlength);		// assuming hop count as distance metric

				// when current connection is served, then for the next connection,vcid 
				// should start from the next vcid value in line
				vcid++;

			}
		
		}
	}

	// forwarding table created and completed
	// [router_id incoming_id incoming_vcid outgoing_id outgoing_vcid]
	for(j = 0; j < num_nodes; j++)
	{
		for(k = 0; k < max_connections; k++)
		{
			if(forwarding_table[j][k][0] == -1)	
			{
				// till no more entries are there for node j in forwarding table
				break;
			}
			fprintf(fw, "%d \t%d \t%d \t%d \t%d\n", j, forwarding_table[j][k][0], forwarding_table[j][k][1], forwarding_table[j][k][2], forwarding_table[j][k][3]);
		}
	}

	fprintf(pathwriter, "%d \t%d", num_connections, admitted_count);
	// creating the path table
	fclose(pathwriter);
	fclose(fw);
	fclose(output);
}

void store_routing_table(char filename[], int num_nodes, int shortest_path1[][max_nodes][max_nodes], int shortest_path2[][max_nodes][max_nodes], long double topology[][max_nodes][2])
{
	// format: [src dest path path_delay path_cost]
	long double path_delay;
	int path_cost;
	int i, j, prev, curr;
	FILE *writer; 

	writer = fopen(filename, "w");

	for(i = 0; i < num_nodes; i++)
	{
		for(j = 0; j < num_nodes; j++)
		{
			if(i != j)
			{
				/* writing the shortest path from node i to j to file */
				fprintf(writer, "%d\t%d\t%d", i, j, i);
				
				path_cost = 0;
				path_delay = 0;
				prev = i;
				curr = shortest_path1[i][j][path_cost];

				while(shortest_path1[i][j][path_cost] != -1)
				{
					fprintf(writer, "-%d", shortest_path1[i][j][path_cost]);
					path_delay += topology[prev][curr][0];
					prev = shortest_path1[i][j][path_cost];
					path_cost++;
					curr = shortest_path1[i][j][path_cost];

				}

				fprintf(writer, "\t\t\t");
				fprintf(writer, "%Lf\t%d\n", path_delay, path_cost);


				/* writing the second shortest path from node i to j to file */
				fprintf(writer, "%d\t%d\t%d", i, j, i);
				
				path_cost = 0;
				path_delay = 0;
				prev = i;
				curr = shortest_path2[i][j][path_cost];

				while(shortest_path2[i][j][path_cost] != -1)
				{
					fprintf(writer, "-%d", shortest_path2[i][j][path_cost]);
					path_delay += topology[prev][curr][0];
					prev = shortest_path2[i][j][path_cost];
					path_cost++;
					curr = shortest_path2[i][j][path_cost];

				}

				fprintf(writer, "\t\t\t");
				fprintf(writer, "%Lf\t%d\n\n", path_delay, path_cost);


			}
			
			
		}
	}

	fclose(writer);
}

int main(int argc, char** argv)
{
	bool file_opened = false;
	int num_nodes, num_edges;
	long double topology[max_nodes][max_nodes][2]; // first value of top[][] has delay, second has capacity
	int cost_path1[max_nodes][max_nodes], cost_path2[max_nodes][max_nodes];
	int parent_path1[max_nodes][max_nodes], parent_path2[max_nodes][max_nodes];
	char top_filename[256], conn_filename[256], routing_filename[256], forwarding_filename[256], paths_file[256], hop_or_dist[16];
	int connections[max_connections][conn_params];
	int forwarding_table[max_nodes][max_connections][conn_params];

	int connection_p, num_connections;

	strcpy(top_filename, argv[2]);
	strcpy(conn_filename, argv[4]);
	strcpy(routing_filename, argv[6]);
	strcpy(forwarding_filename, argv[8]);
	strcpy(paths_file, argv[10]);
	strcpy(hop_or_dist, argv[12]);
	
	connection_p = atoi(argv[14]);

	int status;
	status = remove("shortest_path_2.txt");				// since this file was created using appending data, so deleting the file before every run
	// if(status == 0)
	// 	cout<<"\nPrevious SP2_file removed successfully!";

	file_opened = read_topology(top_filename, &num_nodes, &num_edges, topology);
	if(file_opened == false)
		return 0;


	/* Routing setup */
	memset(cost_path1, -1, sizeof(cost_path1));
	memset(cost_path2, -1, sizeof(cost_path2));
	memset(parent_path1, -1, sizeof(parent_path1));
	memset(parent_path2, -1, sizeof(parent_path2));

	// initializing adjacency matrix


	original_topology(topology, cost_path1, parent_path1, num_nodes, hop_or_dist);
	original_topology(topology, cost_path2, parent_path2, num_nodes, hop_or_dist);

	int i, j;
	
	// for finding shortest path between all pairs of nodes, need to apply Floyd-Warshall

	all_pairs_shortest_path(cost_path1, parent_path1, num_nodes);
	

	int shortest_path1[max_nodes][max_nodes][max_nodes];
	int shortest_path2[max_nodes][max_nodes][max_nodes];

	memset(shortest_path1, -1, sizeof(shortest_path1));
	memset(shortest_path2, -1, sizeof(shortest_path2));

	store_shortest_paths(parent_path1, num_nodes, "1", shortest_path1);

	//print_shortest_paths(shortest_path1,  num_nodes);




	for(i = 0; i < num_nodes; i++)
	{
		for(j = 0; j < num_nodes; j++)
		{
			if(i != j)
			{

				remove_shortest_path(shortest_path1, cost_path2, parent_path2, i, j);

				all_pairs_shortest_path(cost_path2, parent_path2, num_nodes);

				store_second_best_path(parent_path2, shortest_path2, i, j);

				original_topology(topology, cost_path2, parent_path2, num_nodes, hop_or_dist);
			}
			
		}
	}

	//print_shortest_paths(shortest_path2,  num_nodes);

	store_routing_table(routing_filename, num_nodes, shortest_path1, shortest_path2, topology);	// using hop as distance metric only
	/* End of routing setup */


	/* Connection establishment */
	file_opened = read_connections(conn_filename, &num_connections, connections);
	if(file_opened == false)
		return 0;

	memset(forwarding_table, -1, sizeof(forwarding_table));

	connection_establishment(connection_p, num_nodes, paths_file, forwarding_filename, forwarding_table, connections, num_connections, shortest_path1, shortest_path2, topology);
	
}

