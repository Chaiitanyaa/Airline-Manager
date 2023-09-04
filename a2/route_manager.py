#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Wed Feb 8 14:44:33 2023
Based on: https://www.kaggle.com/datasets/arbazmohammad/world-airports-and-airlines-datasets
Sample input: --AIRLINES="airlines.yaml" --AIRPORTS="airports.yaml" --ROUTES="routes.yaml" --QUESTION="q1" --GRAPH_TYPE="bar"
@author: rivera
@author: V00984522
"""
import yaml
import pandas as pd
import sys
import matplotlib.pyplot as plt

def inputData() -> str:
    """Gets the input from command line and stores them in variables

    Parameters
    ----------
    None

    Returns
    -------
    airlines_path: str
        returns the file name that has airlines data
    airports_path: str
        returns the file name that has airports data
    routes_path: str
        returns the file name that has routes data
    question: str
        returns the question that is being asked by the user
    graph_type: str
        returns the type of graph user wants
    """
    args = sys.argv[1:]  # Get all command-line arguments except the script name

    airlines_path = None
    airports_path = None
    routes_path = None
    question = None
    graph_type = None

    for arg in args:
        if arg.startswith('--AIRLINES='):
            airlines_path = arg.split('=')[1].strip('"')
        elif arg.startswith('--AIRPORTS='):
            airports_path = arg.split('=')[1].strip('"')
        elif arg.startswith('--ROUTES='):
            routes_path = arg.split('=')[1].strip('"')
        elif arg.startswith('--QUESTION='):
            question = arg.split('=')[1].strip('"')
        elif arg.startswith('--GRAPH_TYPE='):
            graph_type = arg.split('=')[1].strip('"')

    return airlines_path, airports_path, routes_path, question, graph_type

def readYamlFiles(airlinesData: str, airportsData: str, routesData: str) -> str:
    """Reads the .yaml files that are passed by the user

    Parameters
    ----------
    airlinesData: str
        Contains the Datafile name for airlines file
    airportsData: str
        Contains the Datafile name for airports file
    routesData: str
        Contains the Datafile name for routes file
    Returns
    -------
    merged_df2:
        A dataframe with all of the .yaml files merged together

    """

    # Opening the Yaml files and converting them into a dictionary
    with open(airlinesData, 'r') as f:
        airlines_dict = yaml.safe_load('\n'.join(f.readlines()[1:]))

    with open(airportsData, 'r') as f:
        airports_dict = yaml.safe_load('\n'.join(f.readlines()[1:]))

    with open(routesData, 'r') as f:
        routes_dict = yaml.safe_load('\n'.join(f.readlines()[1:]))

    # Convert the list of dictionaries to a Pandas DataFrame
    airports_df = pd.DataFrame(airports_dict)
    airports_df['airport_country'] = airports_df['airport_country'].str.lstrip()

    routes_df = pd.DataFrame(routes_dict)
    routes_df.columns = routes_df.columns.str.lstrip()

    airlines_df = pd.DataFrame(airlines_dict)
    airlines_df.columns = airlines_df.columns.str.lstrip()

    # Merging the data in the 3 dataframes
    merged_df1 = pd.merge(routes_df, airports_df,left_on='route_to_airport_id', right_on='airport_id', how='left')
    merged_df2 = pd.merge(merged_df1, airlines_df,left_on='route_airline_id', right_on='airline_id', how='left')

    return merged_df2

def barGraph(fileToRead: str, fileToWrite: str, x_axis: str, y_axis: str, title:str) -> None:
    """Creates a bar graph from the inputted file

    Parameters
    ----------
    fileToRead: str
        Contains the name of the file to read from the qestion
    fileToWrite: str
        Contains the name of the file to write from the qestion
    x_axis: str
        Contains the name of to give to the x-axis from the qestion
    y_axis: str
        Contains the name of to give to the y-axis from the qestion
    title: str
        Contains the name of the title to be given to the graph
    Returns
    -------
    None

    """
    data = pd.read_csv(fileToRead)
    x = data['subject']
    y = data['statistic']

    plt.bar(x, y, color='navy', width=0.3)
    plt.title(title)
    plt.xlabel(x_axis)
    plt.xticks(rotation='vertical')
    plt.ylabel(y_axis)
    plt.tight_layout()
    plt.savefig(fileToWrite)

def pieChart(fileToRead:str, fileToWrite:str, title:str) -> None:
    """Creates a Piechart from the inputted file

    Parameters
    ----------
    fileToRead: str
        Contains the name of the file to read from the qestion
    fileToWrite: str
    title: str
        Contains the name of the title to be given to the graph
        
    Returns
    -------
    None

    """
    data = pd.read_csv(fileToRead)
    labels = data['subject']
    sizes = data['statistic']

    plt.rcParams.update({'font.size': 5})
    plt.pie(sizes, labels=labels, autopct='%1.1f%%')

    plt.title(title)

    plt.savefig(fileToWrite)

def q1(airlinesData: str, airportsData: str, routesData: str, graph_type: str) -> None:
    """This function produces the answer for q1

    Parameters
    ----------
    airlinesData: str
        contains the name of the datafile airlines.yaml
    airportsData: str
        contains the name of the datafile airports.yaml
    routesData: str
        contains the name of the datafile routes.yaml
    graph_type: str
        contains information on which graph to make

    Returns
    -------
    None

    """

    # Calling the function readYamlFiles and saving the result in merged_df
    merged_df = readYamlFiles(airlinesData, airportsData, routesData)
    merged_df.loc[:, 'airline_name'] = merged_df['airline_name'] + ' (' + merged_df['airline_icao_unique_code'] + ')'

    # Dropping the fields not requiered by us to answer the question
    merged_df.drop(['airline_country', 'route_from_aiport_id', 'airport_name', 'airport_city','route_airline_id', 'airport_altitude', 'airport_icao_unique_code', 'airline_id'], inplace=True, axis=1)

    # Sorting all the countries with destination as canada
    canada_routes_df = merged_df[merged_df['airport_country'] == 'Canada']

    # Counting the data from airline_name and sorting it by the number of occorences
    answer_df = canada_routes_df.groupby(by=['airline_name'], as_index=False).size().sort_values(by=['size', 'airline_name'], ascending=[False, True]).head(20)

    # Renaming the columns
    answer_df = answer_df.rename(columns={'airline_name': 'subject', 'size': 'statistic'})

    # Print the resulting DataFrame to a .csv file
    answer_df.to_csv("q1.csv", index=False)

    #Graphs of Q1
    if (graph_type == 'bar'):
        csvFile = 'q1.csv'
        nameForPDF = 'q1.pdf'
        xAxis = 'Airline Offering the routes'
        yAxis = 'No. of routes offered to Canada'
        title = 'Top 20 airlines with destinantion as Canada'
        barGraph(csvFile, nameForPDF, xAxis, yAxis, title)

    if (graph_type == 'pie'):
        csvFile = 'q1.csv'
        nameForPDF = 'q1.pdf'
        title = 'Top 20 airlines with destinantion as Canada'
        pieChart(csvFile, nameForPDF, title)

def q2(airlinesData: str, airportsData: str, routesData: str, graph_type: str) -> None:
    """This function produces the answer for q2

    Parameters
    ----------
    airlinesData: str
        contains the name of the datafile airlines.yaml
    airportsData: str
        contains the name of the datafile airports.yaml
    routesData: str
        contains the name of the datafile routes.yaml
    graph_type: str
        contains information on which graph to make
        
    Returns
    -------
    None

    """

    # Calling the function readYamlFiles and saving the result in merged_df
    merged_df = readYamlFiles(airlinesData, airportsData, routesData)

    # Dropping the fields not requiered by us to answer the question
    merged_df.drop(['airline_country', 'airline_icao_unique_code', 'route_from_aiport_id', 'airport_name','airport_city', 'route_airline_id', 'airport_altitude', 'airport_icao_unique_code'], inplace=True, axis=1)

    # Print the resulting DataFrame to a .csv file
    answer = merged_df.groupby(['airport_country'], as_index=False).size().sort_values(by=['size', 'airport_country'], ascending=[True, True]).head(30)

    # Renaming the columns
    answer = answer.rename(columns={'airport_country': 'subject', 'size': 'statistic'})

    # Print the resulting DataFrame to a .csv file
    answer.to_csv("q2.csv", index=False)

    # Graphs of Q2
    if (graph_type == 'bar'):
        csvFile = 'q2.csv'
        nameForPDF = 'q2.pdf'
        xAxis = 'Destinition Coutnry'
        yAxis = 'Number of appearances'
        title = 'Top 30 countries with least appearances as destination country'
        barGraph(csvFile, nameForPDF, xAxis, yAxis, title)

    if (graph_type == 'pie'):
        csvFile = 'q2.csv'
        nameForPDF = 'q2.pdf'
        title = 'Top 30 countries with least appearances as destination country'
        pieChart(csvFile, nameForPDF, title)

def q3(airlinesData: str, airportsData: str, routesData: str, graph_type: str) -> None:
    """This function produces the answer for q3

    Parameters
    ----------
    airlinesData: str
        contains the name of the datafile airlines.yaml
    airportsData: str
        contains the name of the datafile airports.yaml
    routesData: str
        contains the name of the datafile routes.yaml
    graph_type: str
        contains information on which graph to make
        
    Returns
    -------
    None

    """
    # Calling the function readYamlFiles and saving the result in merged_df(pd.DataFrame)
    merged_df = readYamlFiles(airlinesData, airportsData, routesData)

    # Setting the data in the requiered format
    merged_df['airport_name'] = merged_df['airport_name'].astype(str) + ' (' + merged_df['airport_icao_unique_code'].astype(str) + '), ' + merged_df['airport_city'].astype(str) + ', ' + merged_df['airport_country'].astype(str)

    # Dropping the fields not requiered by us to answer the question
    merged_df.drop(['airline_country', 'airline_icao_unique_code', 'route_from_aiport_id', 'route_airline_id','airport_altitude', 'route_to_airport_id', 'airport_id', 'airline_name'], inplace=True, axis=1)

    # Grouping the field, counting the no of times it comes and then arranging it in order
    destination_df = merged_df.groupby(by=['airport_name'], as_index=False).size().sort_values(by=['size', 'airport_name'], ascending=[False, True]).head(10)

    # Renaming the columns
    answer_df = destination_df.rename(columns={'airport_name': 'subject', 'size': 'statistic'})

    # Print the resulting DataFrame to a .csv file
    answer_df.to_csv("q3.csv", index=False)

    #Graphs of Q3
    if (graph_type == 'bar'):
        csvFile = 'q3.csv'
        nameForPDF = 'q3.pdf'
        xAxis = 'Airport Name'
        yAxis = 'Number of appearances'
        title = 'Top 10 destination airports'
        barGraph(csvFile, nameForPDF, xAxis, yAxis, title)

    if (graph_type == 'pie'):
        csvFile = 'q3.csv'
        nameForPDF = 'q3.pdf'
        title = 'Top 10 destination airports'
        pieChart(csvFile, nameForPDF, title)

def q4(airlinesData: str, airportsData: str, routesData: str, graph_type: str) -> None:
    """This function produces the answer for q4

    Parameters
    ----------
    airlinesData: str
        contains the name of the datafile airlines.yaml
    airportsData: str
        contains the name of the datafile airports.yaml
    routesData: str
        contains the name of the datafile routes.yaml
    graph_type: str
        contains information on which graph to make
        
    Returns
    -------
    None

    """

    # Calling the function readYamlFiles and saving the result in merged_df
    merged_df = readYamlFiles(airlinesData, airportsData, routesData)

    # Dropping the fields not requiered by us to answer the question
    merged_df.drop(['route_from_aiport_id', 'route_airline_id', 'airport_altitude','route_to_airport_id', 'airport_id'], inplace=True, axis=1)

    # Grouping the field, counting the no of times it comes and then arranging it in order
    destination_df = merged_df.groupby(['airport_city', 'airport_country'], as_index=False).size().sort_values(by=['size', 'airport_city'], ascending=[False, True]).head(15)

    # Setting the data in the requiered format
    destination_df.loc[:, 'airport_city'] = destination_df['airport_city'] + ', ' + destination_df['airport_country']

    # Dropping the fields not requiered by us to answer the question
    destination_df.drop(['airport_country'], inplace=True, axis=1)

    # Renaming the columns
    answer_df = destination_df.rename(columns={'airport_city': 'subject', 'size': 'statistic'})

    # Print the resulting DataFrame to a .csv file
    answer_df.to_csv("q4.csv", index=False)

    #Graphs of Q4
    if (graph_type == 'bar'):
        csvFile = 'q4.csv'
        nameForPDF = 'q4.pdf'
        xAxis = 'Airport City'
        yAxis = 'Number of appearances'
        title = 'Top 15 destination cities'
        barGraph(csvFile, nameForPDF, xAxis, yAxis, title)

    if (graph_type == 'pie'):
        csvFile = 'q4.csv'
        nameForPDF = 'q4.pdf'
        title = 'Top 15 destination cities'
        pieChart(csvFile, nameForPDF, title)


def q5(airportsData: str, routesData: str, graph_type: str) -> None:
    """This function produces the answer for q5

    Parameters
    ----------
    airportsData: str
        contains the name of the datafile airports.yaml
    routesData: str
        contains the name of the datafile routes.yaml
    graph_type: str
        contains information on which graph to make
        
    Returns
    -------
    None

    """

    with open(airportsData) as f:
        airports_dict = yaml.safe_load('\n'.join(f.readlines()[1:]))

    with open(routesData) as f:
        routes_dict = yaml.safe_load('\n'.join(f.readlines()[1:]))

    # Create pandas DataFrames for airports and routes
    airports_df = pd.DataFrame(airports_dict)
    routes_df = pd.DataFrame(routes_dict)

    # Join the airports and routes DataFrames to get destination airport altitudes
    merged_df = pd.merge(routes_df, airports_df,left_on='route_to_airport_id', right_on='airport_id')
    merged_df = merged_df.rename(columns={'airport_altitude': 'destinition_altitude', 'airport_icao_unique_code': 'dest_code'})

    # Sorting the data by only Canada as destination county
    merged_df = merged_df[merged_df['airport_country'] == 'Canada']

    # Dropping the fields not requiered by us to answer the question
    merged_df.drop(['airport_id', 'airport_city', 'airport_country','route_to_airport_id'], inplace=True, axis=1)

    # Join the previously merged destination and routes DataFrames to get origin airport altitudes
    routes_with_altitudes = pd.merge(merged_df, airports_df, left_on='route_from_aiport_id', right_on='airport_id')
    routes_with_altitudes = routes_with_altitudes.rename(columns={'airport_altitude': 'origin_altitude', 'airport_icao_unique_code': 'origin_code'})

    # Changing the datafields datatype to int from str 
    routes_with_altitudes['destinition_altitude'] = pd.to_numeric(routes_with_altitudes['destinition_altitude'], errors='coerce')
    routes_with_altitudes['origin_altitude'] = pd.to_numeric(routes_with_altitudes['origin_altitude'], errors='coerce')

    # Calculating the altitude difference
    routes_with_altitudes['altitude_diff'] = routes_with_altitudes['destinition_altitude'] - routes_with_altitudes['origin_altitude']
    routes_with_altitudes['altitude_diff'] = routes_with_altitudes['altitude_diff'].abs()

    # Sorting the data by only Canada as destination county
    routes_with_altitudes = routes_with_altitudes[routes_with_altitudes['airport_country'] == 'Canada']

    # Dropping the fields not requiered by us to answer the question    
    routes_with_altitudes.drop(['route_airline_id', 'airport_id', 'airport_city', 'route_from_aiport_id','airport_name_x', 'airport_name_y', 'airport_country'], inplace=True, axis=1)

    # Setting the data in the requiered format
    routes_with_altitudes.loc[:, 'RoutePairDtoO'] = routes_with_altitudes['origin_code'] + '-' + routes_with_altitudes['dest_code']

    # Grouping the field, counting the no of times it comes and then arranging it in order
    routes_with_altitudes = routes_with_altitudes.groupby(by=['RoutePairDtoO', 'altitude_diff'], as_index=False).size().sort_values(by=['altitude_diff', 'RoutePairDtoO'], ascending=[False, False]).head(10)
    
    # Dropping all the duplicate routes
    routes_with_altitudes = routes_with_altitudes.drop_duplicates(subset=['RoutePairDtoO']).reset_index(drop=True)
    routes_with_altitudes.drop(['size'], inplace=True, axis=1)

    # Renaming the columns
    answer_df = routes_with_altitudes.rename(columns={'RoutePairDtoO': 'subject', 'altitude_diff': 'statistic'})

    # Print the resulting DataFrame to a .csv file
    answer_df.to_csv("q5.csv", index=False)

    #Graphs of Q5
    if (graph_type == 'bar'):
        csvFile = 'q5.csv'
        nameForPDF = 'q5.pdf'
        xAxis = 'Route'
        yAxis = 'Altitude Difference'
        title = 'unique top 10 Canadian routes with most difference between the destination altitude and the origin altitude'
        barGraph(csvFile, nameForPDF, xAxis, yAxis, title)

    if (graph_type == 'pie'):
        csvFile = 'q5.csv'
        nameForPDF = 'q5.pdf'
        title = 'unique top 10 Canadian routes with most difference between the destination altitude and the origin altitude'
        pieChart(csvFile, nameForPDF, title)


def main():
    """Main entry point of the program."""

    airlines_path, airports_path, routes_path, question, graph_type = inputData()

    if (question == "q1"):
        q1(airlines_path, airports_path, routes_path, graph_type)

    elif (question == "q2"):
        q2(airlines_path, airports_path, routes_path, graph_type)

    elif (question == "q3"):
        q3(airlines_path, airports_path, routes_path, graph_type)

    elif (question == "q4"):
        q4(airlines_path, airports_path, routes_path, graph_type)

    elif (question == "q5"):
        q5(airports_path, routes_path, graph_type)


if __name__ == '__main__':
    main()
