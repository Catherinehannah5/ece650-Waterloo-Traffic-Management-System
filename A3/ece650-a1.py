import math,re,sys
class Graph():
	v=[]
	points_index={}
	Edges_db=[]
	streets_name=[]
	street_dict={}
	lines=[]
	intersections=[]
	lines_intersect={}
	final_edges=[]
	temp=set()
	p_i=0
	def __init__(self,streets):
		self.streets_name=list(streets.keys())
		self.street_dict=streets
		#print(self.street_dict)
	def find_parallel(self,l1,l2):

		if self.get_gradient(l1) != self.get_gradient(l2):
			return False
		return True
	def get_intersect(self,line):
		return line[0][1] - (self.get_gradient(line)*line[0][0])
	def get_gradient(self,l):
		m = None
		if l[0][0] != l[1][0]:
			m = (1./(l[0][0]-l[1][0]))*(l[0][1] - l[1][1])
			return m	
	def find_line_intersection(self,l1,l2):
		if not self.find_parallel(l1,l2):
			if self.get_gradient(l1) is not None and self.get_gradient(l2) is not None:
				x = (1./(self.get_gradient(l1) - self.get_gradient(l2))) * (self.get_intersect(l2) - self.get_intersect(l1))
				y = (self.get_gradient(l1)*x) + self.get_intersect(l1)
				#print(x, y)
			else:
				if self.get_gradient(l1) is None:
					x = l1[0][0]
					y = (self.get_gradient(l2)*x) + self.get_intersect(l2)
				elif self.get_gradient(l2) is None:
					x = l2[0][0]
					y = (self.get_gradient(l1)*x) + self.get_intersect(l1)
			x = float("{0:.2f}".format(x))
			y = float("{0:.2f}".format(y))
			intersect_pt = [(x,y)]
			#print(intersect_pt)
			return (x,y)
		else:
			return False



	def find_distance(self,x1, y1, x2, y2):
		dist = math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)
		return dist
	def if_crosses(self,l1,l2):
		
		if not self.find_parallel(l1,l2):
			intersect_pt = self.find_line_intersection(l1,l2)
			
			x = intersect_pt[0]
			y = intersect_pt[1]
			#print(x,y)
			xranges = [max(min(l1[0][0],l1[1][0]),min(l2[0][0],l2[1][0])),min(max(l1[0][0],l1[1][0]),max(l2[0][0],l2[1][0]))]
			if min(xranges) <= x <= max(xranges):
				dist = self.find_distance(l1[0][0], l1[0][1], l1[1][0], l1[1][1])
				dist1 = self.find_distance(l1[0][0], l1[0][1], x, y)
				dist2 = self.find_distance(x, y, l1[1][0], l1[1][1])
                                    
				if dist1 > dist or dist2 > dist:
					pass
					
				else:
					
					
						#print(l1,l2)
						if(intersect_pt not in l1 and intersect_pt not in l2):
								
							if intersect_pt not in self.intersections:
								self.intersections.append(intersect_pt)
								self.temp.add(intersect_pt)
							self.temp.add(l1[0])
							self.temp.add(l1[1])
							self.temp.add(l2[0])
							self.temp.add(l2[1])

							
							if tuple(l1) not in self.lines_intersect:
								self.lines_intersect[tuple(l1)]=[intersect_pt]
							else:
								if intersect_pt not in self.lines_intersect[tuple(l1)]:
									self.lines_intersect[tuple(l1)].append(intersect_pt)
							if tuple(l2) not in self.lines_intersect:
								self.lines_intersect[tuple(l2)]=[intersect_pt]
							else:
								if intersect_pt not in self.lines_intersect[tuple(l2)]:
									self.lines_intersect[tuple(l2)].append(intersect_pt)
					
							
							
				
				return True   
			else:
				#print("Lines do not intersect!")
				return False
		else:
			#print("Lines are parallel!")
			slope1 = 0
			slope2 = 0

			try:
				slope1 = (l1[1][1] - l1[0][1]) / (l1[1][0] - l1[0][0])
				slope2 = (l2[1][1] - l2[0][1]) / (l2[1][0] - l2[0][0])
				y_int1 = l1[0][1] - (slope1 * l1[0][0])
				y_int2 = l2[0][1] - (slope2 * l2[0][0])
			except:
				slope1 = 0
				slope2 = 0        
				y_int1 = l1[0][0] - (slope1 * l1[0][0])
				y_int2 = l2[0][0] - (slope2 * l2[0][0])
            
			if (slope1 == slope2) and (y_int1 == y_int2):
				#print("Lines overlap")
				p=l1[0]
				if(self.in_range(p,l2)):
					self.add_intersect(p,l2)
				
				p=l1[1]
				if(self.in_range(p,l2)):
					self.add_intersect(p,l2)

				p=l2[0]
				if(self.in_range(p,l1)):
					self.add_intersect(p,l1)
				p=l2[1]
				if(self.in_range(p,l1)):
					self.add_intersect(p,l1)
				#for i in range(4):
				#	if()
				#	pass
				          
				return True
			return False
	def add_intersect(self,intersect_pt,l1):
		self.temp.add(l1[0])
		self.temp.add(l1[1]) 
		if intersect_pt not in self.intersections:
			self.intersections.append(intersect_pt)
		if tuple(l1) not in self.lines_intersect:
			self.lines_intersect[tuple(l1)]=[intersect_pt]
		else:
			if intersect_pt not in self.lines_intersect[tuple(l1)]:
				self.lines_intersect[tuple(l1)].append(intersect_pt)
		
	def tofloat(self):
		for k,v in self.street_dict.items():
			length=len(v)
			for i in range(length):
				#print(isinstance(v[0],tuple))
				if(isinstance(v[0],tuple)):
					break
				
				cor=v[0].split(',')
				x = float(cor[0][1:])
				y = float(cor[1][:len(cor[1])-1])
				
				self.street_dict[k].remove(v[0])
				self.street_dict[k].append((x,y))
	def in_range(self,p,l):
        
		if(p[0]==l[0][0] and p[0]==l[1][0]):
            
			if(p[1]>=l[0][1] and p[1]<=l[1][1]):
				return True;
			elif(p[1]>=l[1][1] and p[1]<=l[0][1]):
				return True;
			return False;
            
		if(p[0]>=l[0][0] and p[0]<=l[1][0]):
			return True;
		elif(p[0]>=l[1][0] and p[0]<=l[0][0]):
			return True;
		return False;
		
	def set_lines(self):
		
		for k,v in self.street_dict.items():
			for i in range(len(v)-1):
				self.lines.append( [ (v[i][0],v[i][1]) , (v[i+1][0],v[i+1][1]) ] )
	def get_vertices(self):
		self.intersections=[]
		self.lines=[]
		self.lines_intersect={}
		self.points_index={}
		self.final_edges=[]
		self.temp.clear()

		self.tofloat()
		
		self.set_lines()
		
		
		
		for i in range(len(self.lines)):
			for j in range(i+1,len(self.lines)):
				self.if_crosses(self.lines[i],self.lines[j])
		#print(self.intersections)
		#print(self.lines_intersect)
		for k,v in self.lines_intersect.items():
			#print(k,v)
			#iterator = 0 
			if((k[0],v[0]) not in self.final_edges and (v[0],k[0]) not in self.final_edges and (v[0]!=k[0])):
				self.final_edges.append((k[0],v[0]))
			for i in range(0,len(v)-1):
				if((v[i],v[i+1]) not in self.final_edges and (v[i+1],v[i]) not in self.final_edges  and (v[i]!=v[i+1])):				
					self.final_edges.append((v[i],v[i+1]))
			if((k[1],v[len(v)-1]) not in self.final_edges and (v[len(v)-1],k[1]) not in self.final_edges  and (v[len(v)-1]!=k[1])):
				self.final_edges.append((k[1],v[len(v)-1]))
		#print()
		
		#print("V = {")
		k=1
		#count = 0
		newcount = len(self.temp)
		for v in self.temp:
			#print("{0} : ({1},{2})".format(k,float(v[0]) ,float(v[1]) )) 
			k+=1
			#count = count + 1
			self.points_index[k]=v;
		#print("}")
		#print("V",count)
		print("V",newcount)
		key_list = list(self.points_index.keys()) 
		val_list = list(self.points_index.values()) 

		ii = 0    
		sys.stdout.write("E {")
		for i in self.final_edges:
			#if ii < len(self.final_edges):
			comma = "" if (ii == len(self.final_edges) - 1) else ","

			#sys.stdout.write("<{0},{1}>".format(val_list.index(i[0])+1 , val_list.index(i[1])+1) + comma)
			sys.stdout.write("<{0},{1}>".format(val_list.index(i[0]) , val_list.index(i[1])) + comma)
			ii = ii+1

		print("}")


		#print("E = {")
		#for i in self.final_edges:
			#print("<" + str(val_list(i[0])+1) , str(val_list.index(i[1])+1) , ">")
		#print("}")
		#for i in self.final_edges:
		#	print(i)
		

		
	
		
	
def main():
    streets = {}
	#dummy_in = ['a "weber st" (2,-1) (2,2) (5,5) (5,6) (3,8)' , 'a "king st" (4,2) (4,8)' , 'a "dave" (1,4) (5,8)', 'g' , 'c "weber st" (2,1) (2,2)' , 'g']
	#dummy_in = ['a "weber st" (2,-1) (2,2) (5,5) (5,6) (3,8)' , 'a "king st" (4,2) (4,8)' , 'a "dave" (1,4) (5,8)', 'g']
	#dummy_in = ['a "weber st" (4,2) (8,4)' , 'a "king st" (6,3) (16,8)' , 'g' ]
    #a=-1
    while (True):
        #a+=1            
        try:
            inp = input()
            #inp =dummy_in[a]
        except EOFError:
            sys.exit(0)
            break
        if inp == '':
            sys.exit(0)
            break
		#a+=1;

 
        command = r'([acrg])((?: +?"[a-zA-Z ]+?" *?))?((?:\([-]?[0-9]+?,[-]?[0-9]+?\) *?)*)?$'
        group_var = re.match(command,inp)
        if group_var:
            operation = group_var.group(1)
            street_name = group_var.group(2)
            raw_coorlist = group_var.group(3)
           
            coorlist = re.findall(r'\([-]?[0-9]+,[-]?[0-9]+\)', raw_coorlist)
			#print(coorlist)

        else:
            print("Error: The input does not follow the correct format")
            continue

        
        try:
            if operation == 'a':

                if len(street_name) < 1:
                    print("Error! Street name is not entered")
                    continue
     
                street_name = street_name.replace('"', '')
                street_name = street_name.strip()
                street_name = str(street_name).lower()
                if street_name in streets:
                    print("Error: Street name" + street_name + "already exists!")
                    continue

                if len(coorlist) < 2:
                    print("Error! There should be minimum 2 coordinates")
                    continue
                
                streets[street_name] = coorlist            
            
            elif operation == 'c':
                if len(street_name) < 1:
                    print("Error! Street name is not entered")
                    continue
                
                street_name = street_name.replace('"', '')
                street_name = street_name.lstrip()
                street_name = street_name.rstrip()
                street_name = str(street_name).lower()
                
                if street_name not in streets:
                    print("Error :Cannot change a street that doesn't exit!")
                    continue
                
                if len(coorlist) < 2:
                    print("Error! There should be minimum 2 coordinates")
                    continue                
                streets[street_name] = coorlist
                #print(streets)

            
            elif operation == 'r':
                if len(street_name) < 1:
                    print("Error! Street name is not entered")
                    continue
                street_name = street_name.replace('"', '')
                street_name = street_name.lstrip()
                street_name = street_name.rstrip()
                street_name = str(street_name).lower()
                
                if street_name not in streets:
                    print("Error: Cannot remove a street that doesn't exit")
                    continue
                del streets[street_name]

            
            elif operation == 'g':
                if len(streets) == 0:
                    print("Error: No streets")
                    continue
                g = Graph(streets)
                g.get_vertices()
        except Exception as exp:
            print("Error: " + str(exp) + "\n")
            pass
    #sys.exit(0)
if __name__ == '__main__':
	main()
