import java.io.FileWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;


public class ZooFileWriter {


	public static String padLeft(String str, int leng, String stringpadVal) {
        for (int i = str.length(); i < leng; i++)
            str = stringpadVal + str;
        return str;
    }	

	public static String padRight(String str, int leng, String stringpadVal) {
        for (int i = str.length(); i < leng; i++)
            str = str + stringpadVal;
        return str;
    }	
	
	
	public static void createZooFile() {
		 try{  
			 
			char entermore = 'Y';
			 
	           FileWriter fw=new FileWriter("zoodata.txt");    
	           BufferedReader reader =
	                   new BufferedReader(new InputStreamReader(System.in));
	           String animalName,trackNumber,animalType,animalSubType,eggs,Nurse;
           
	           do
	           {
		           System.out.println("Track Number: ");
		           trackNumber = reader.readLine();
		           trackNumber = padLeft(trackNumber, 6,"0");
		
		           
		           System.out.println("Enter Animal Name: ");
		           animalName = reader.readLine();
		           animalName = padRight(animalName, 15," ");
		           System.out.println("Enter Animal Type: ");
		           animalType = reader.readLine();
		           animalType = padRight(animalType, 15," ");
		           System.out.println("Enter Animal Sub-type: ");
		           animalSubType = reader.readLine();
		           animalSubType = padRight(animalSubType, 15," ");
		           System.out.println("Enter Number of Eggs: ");
		           eggs = reader.readLine();	           
		           System.out.println("Enter 1 if Nursing, 0 if not: ");
		           Nurse = reader.readLine();	           
		           
		           fw.write(trackNumber+animalName+animalType+animalSubType+eggs+" "+Nurse+"\n");   
		           
		           System.out.println("Enter more data? (Y for yes)");
				   entermore = (char)reader.read();
				   reader.readLine();
	           }
	           while (entermore == 'Y' || entermore == 'y');
	           
	           fw.close();    
	          }catch(Exception e){System.out.println(e);}    
	          System.out.println("The file has been successfully created...");    
	}
	public static void main(String[] args) {	
		createZooFile();
	}
}
