
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import junit.framework.TestCase;
import java.util.concurrent.ThreadLocalRandom;

/**
 * Performs Validation Test for url validations.
 *
 * @version $Revision: 1128446 $ $Date: 2011-05-27 13:29:27 -0700 (Fri, 27 May 2011) $
 */

public class UrlValidatorTest extends TestCase {

	private boolean printStatus = true;
	private boolean printIndex = false;//print index that indicates current scheme,host,port,path, query test were using.

	public UrlValidatorTest(String testName) {
		super(testName);
	}

	public void testManualTest()
	{
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		System.out.println(urlVal.isValid("http://www.amazon.com"));
	}


	public void testYourFirstPartition()
	{
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		boolean expected, result;
		String url;
		
		// Test the scheme
		expected = true;
		url = "ftp://go.cc/t123";
		//System.out.println("Testing the URL scheme:");
		result = urlVal.isValid(url);
		assertEquals(url, expected, result);
		
		url = "http:/go.cc/t123";
		expected = false;
		result = urlVal.isValid(url);
		assertEquals(url, expected, result);
		
	}

	public void testYourSecondPartition(){
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		boolean expected, result;
		String url;
		
		// Test the authority
		expected = true;
		url = "i3://go.cc/helloworld";
		//System.out.println("Testing the URL authority:");
		result = urlVal.isValid(url);
		assertEquals(url, expected, result);
		
		url = "i3://256.256.256.256/helloworld";
		expected = false;
		result = urlVal.isValid(url);
		assertEquals(url, expected, result); // assertion fails
	}
	
	public void testYourThirdPartition() {
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		boolean expected, result;
		String url;
		
		//Test the path
		expected = true;
		url = "ieee://xyz.xyz/path/to/somewhere";
		result = urlVal.isValid(url);
		assertEquals(url, expected, result);
		
		url = "ieee://xyz.xyz/..";
		expected = false;
		result = urlVal.isValid(url);
		assertEquals(url, expected, result);
	}
	
	public void testYourFourthPartition() {
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		boolean expected, result;
		String url;
		
		//Test the query
		expected = true;
		url = "http://google.com?key=value";
		result = urlVal.isValid(url);
		assertEquals(url, expected, result);
	}
	
	public void testYourFifthPartition() {
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		boolean expected, result;
		String url;
		
		//Test the port
		expected = true;
		url = "http://google.com:65636";
		result = urlVal.isValid(url);
		assertEquals(url, expected, result);
		
		url = "http://google.com:-1";
		expected = false;
		result = urlVal.isValid(url);
		assertEquals(url, expected, result);
	}

	public void testIsValid()
	{
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		int partnum = 5; // # of url parts we want to test
		int loopnum = 10; //10000

		for(int i = 0; i<loopnum; i++) { 
			StringBuffer testBuffer = new StringBuffer();  
			boolean expected = true;
			int[] randomPick = randomIndex();

			ResultPair component;
			// pick element for each url compoenents
			for( int j = 0; j < partnum; j++){
				component = (ResultPair) testUrlParts[j][randomPick[j]];
				testBuffer.append(component.item);                               
				expected &= component.valid;  
				if (printStatus) {
					System.out.println(component.item + " -> " + component.valid);
				}
			}
			String url = testBuffer.toString();                                   
			boolean result = urlVal.isValid(url);  
			if(result!=expected){                                                    
				System.out.println(url);                                          
			}
			assertEquals(url, expected, result);     
		}
	}


	public void testAnyOtherUnitTest()
	{

	}

	public int[] randomIndex() {
		int min = 0;
		int[] randomIndex = new int[5];
		for (int i = 0; i < 5; i++){
			randomIndex[i] = (ThreadLocalRandom.current().nextInt(testUrlParts[i].length));
		}
		if ( printIndex == true){
			System.out.print("Testing index: {");
			for (int j = 0; j < 5; j++){
				System.out.print(randomIndex[j]+",");
			}
			System.out.print("}\n");
		}
		return randomIndex;
	}


	/**
	 * Create set of tests by taking the testUrlXXX arrays and
	 * running through all possible permutations of their combinations.
	 *
	 * @param testObjects Used to create a url.
	 */
	ResultPair[] testUrlSchemeNew = {
		new ResultPair("ftp://", true),
		new ResultPair("h3t://", true),
		new ResultPair("http://",true),
		new ResultPair("ieee://",true),
		new ResultPair("http:/", false),
		new ResultPair("i3://",true)
	};

	ResultPair[] testUrlAuthorityNew = {
		new ResultPair("google.com", true),
		new ResultPair("172.217.11.164", true),
		new ResultPair("256.256.256.256", false),
		new ResultPair("go.cc", true),
		new ResultPair("172.217.11.164", true)
	};

	ResultPair[] testPathNew = {
		new ResultPair("/test1", true),
		new ResultPair("/path/to/somewhere", true),
		new ResultPair("/helloworld", true),
		new ResultPair("/hello#world", true),
		new ResultPair("/..", false),
		new ResultPair("", true)
	};

	ResultPair[] testUrlQueryNew = {
		new ResultPair("?key=value", true),
		new ResultPair("", true)
	};

	ResultPair[] testUrlPortNew = {
		new ResultPair(":12345", true),
		new ResultPair(":65636", true),
		new ResultPair("", true)
	};

	/*
	Object[][] testUrlParts = {testUrlScheme, testUrlAuthority, testUrlPort,
		testPath,testUrlQuery}; 
		*/

	Object[][] testUrlParts = {testUrlSchemeNew, testUrlAuthorityNew, testUrlPortNew,
		testPathNew,testUrlQueryNew}; 
}
/*
	ResultPair[] testUrlScheme = {new ResultPair("http://", true),              
		new ResultPair("ftp://", true),                                
		new ResultPair("h3t://", true),                 
		new ResultPair("3ht://", false),                
		new ResultPair("http:/", false),                
		new ResultPair("http:", false),                 
		new ResultPair("http/", false),                 
		new ResultPair("://", false),                   
		new ResultPair("", true)};  


	ResultPair[] testUrlPort = {new ResultPair(":80", true),                    
		new ResultPair(":65535", true),                   
		new ResultPair(":0", true),                       
		new ResultPair("", true),                         
		new ResultPair(":-1", false),                     
		new ResultPair(":65636", true),                   
		new ResultPair(":65a", false)                     
	};                                           

	ResultPair[] testUrlQuery = {new ResultPair("?action=view",true),                         
		new ResultPair("?action=edit&mode=up", true),    
		new ResultPair("", true)                         
	}; 

	ResultPair[] testUrlAuthority = {new ResultPair("www.google.com", true),    
		new ResultPair("go.com", true),              
		new ResultPair("go.au", true),               
		new ResultPair("0.0.0.0", true),             
		new ResultPair("255.255.255.255", true),     
		new ResultPair("256.256.256.256", false),    
		new ResultPair("255.com", true),             
		new ResultPair("1.2.3.4.5", false),          
		new ResultPair("1.2.3.4.", false),           
		new ResultPair("1.2.3", false),              
		new ResultPair(".1.2.3.4", false),           
		new ResultPair("go.a", false),               
		new ResultPair("go.a1a", false),             
		new ResultPair("go.cc", true),               
		new ResultPair("go.1aa", false),             
		new ResultPair("aaa.", false),               
		new ResultPair(".aaa", false),               
		new ResultPair("aaa", false),                
		new ResultPair("", false)      };

	ResultPair[] testUrlAuthority = {new ResultPair("www.google.com", true),    
		new ResultPair("go.com", true),              
		new ResultPair("go.au", true),               
		new ResultPair("0.0.0.0", true),             
		new ResultPair("255.255.255.255", true),     
		new ResultPair("256.256.256.256", false),    
		new ResultPair("255.com", true),             
		new ResultPair("1.2.3.4.5", false),          
		new ResultPair("1.2.3.4.", false),           
		new ResultPair("1.2.3", false),              
		new ResultPair(".1.2.3.4", false),           
		new ResultPair("go.a", false),               
		new ResultPair("go.a1a", false),             
		new ResultPair("go.cc", true),               
		new ResultPair("go.1aa", false),             
		new ResultPair("aaa.", false),               
		new ResultPair(".aaa", false),               
		new ResultPair("aaa", false),                
		new ResultPair("", false)      };

	ResultPair[] testPath = {new ResultPair("/test1", true),                    
		new ResultPair("/t123", true),                       
		new ResultPair("/$23", true),                        
		new ResultPair("/..", false),                        
		new ResultPair("/../", false),                       
		new ResultPair("/test1/", true),                     
		new ResultPair("", true),                            
		new ResultPair("/test1/file", true),                 
		new ResultPair("/..//file", false),                                 
		new ResultPair("/test1//file", false)                
	};  
*/
