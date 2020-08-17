import os
from bs4 import BeautifulSoup
def outcsv():
	with open('class_list.csv', 'w') as outfile:
	    outfile.write('aquarium,0\n')
	    outfile.write('bottle,1\n')
	    outfile.write('bowl,2\n')
	    outfile.write('box,3\n')
	    outfile.write('bucket,4\n')
	    outfile.write('plastic_bag,5\n')
	    outfile.write('plate,6\n')
	    outfile.write('styrofoam,7\n')
	    outfile.write('tire,8\n')
	    outfile.write('toilet,9\n')
	    outfile.write('tub,10\n')
	    outfile.write('washing_machine,11\n')
	    outfile.write('water_tower,12\n')

def getid():
	path = 'C:\\Users\\ASUS\\Downloads\\train_cdc\\train_annotations'
	files = []
	# r=root, d=directories, f = files
	for r, d, f in os.walk(path):
	    for file in f:
	        if '.xml' in file:
	            files.append(file)
	return files
def annot(filename):
	text = []
	with open(filename,'r') as fp:
		content = fp.read()
		soup = BeautifulSoup(content,features="lxml")
		a =  soup.find_all('filename')
		a= a[0].text

		b = soup.find_all('width')
		b= int(b[0].text)

		c = soup.find_all('height')
		c = int(c[0].text)

		d = soup.find_all('depth')
		e = soup.find_all('object')
		if e:
			for box in e:
				name = box.find_all('name')[0].text
				x = int(box.find_all('xmin')[0].text)
				y = int(box.find_all('ymin')[0].text)
				xp = int(box.find_all('xmax')[0].text)
				yp = int(box.find_all('ymax')[0].text)
				s = 'coco/images/train_images/'+ a + ',' + str(x) + ',' + str(y)  + ',' + str(xp)  + ','+str(yp) + ','+name + '\n'
				text.append(s)
				# print(s)
		else:
			text.append('coco/images/train_images/'+ a +',,,,,\n')
		return text
# outcsv()
xml = getid()

c = 0
d = 0
with open('train_annots.csv', 'w') as outfile:
	for data in xml:
		
		out  = annot(data)
		if out:
			d+= 1
			for text in out:
				outfile.write(text)
				c +=1 

	print(c,d)#7946 2671
