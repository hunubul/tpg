#version 330 core
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D frameBuffer;
uniform sampler2D ASCII;
uniform sampler2D ASCIIBrightness;

const vec2 pixelSize = vec2(8.0,8.0);
const float FontX = 8.0;
const float FontY = 8.0;
const float FontSize = 256.0;

void main()
{
	vec2 _viewport = vec2(800.0,600.0);
	vec2 invViewport = vec2(1.0) / _viewport;
	vec4 sum = vec4(0.0);
	vec2 xyClamped = vec2(
		Texcoord.x*800-mod(Texcoord.x*800,FontX),
		Texcoord.y*600-mod(Texcoord.y*600,FontY)
	);
	
	float weight[64];

	for(float x=0.0;x<FontX;x++) {
		for(float y=0.0;y<FontY;y++) {
			vec2 offset = vec2(x,y);
			vec4 temp = texture(frameBuffer, (xyClamped+offset)*invViewport);
			sum += temp;
			weight[int(FontY)*int(y)+int(x)] = (temp.x+temp.y+temp.z)*temp.w/3.0;
		}
	}
	
	vec4 average = sum / vec4(FontX*FontY);
	float brightness = (average.x+average.y+average.z)/3.0;
	
	bool Draw = false;
	float index=0.0;
	float txtCoordX=mod(Texcoord.x*800,FontX)*1/FontX;
	float txtCoordY=mod(Texcoord.y*600,FontY)*1/(FontY*FontSize);
	
	vec4 texture1,texture2;
	float maxWeight = 0.0;
	float currWeight = 0.0;

	for(float i=0.0;i<FontSize;i++) {
		
		currWeight = 0.0;
		for(int x=0;x<int(FontX);x++) {
			for(int y=0;y<int(FontY);y++) {
				currWeight+=(1.0 - abs( weight[int(FontY)*y+x] - texture(ASCII,vec2(x*1/FontX,i*1/FontSize+y*1/(FontY*FontSize))).x ) )*255.0;
			}
		}
		if(maxWeight < currWeight) {
			maxWeight = currWeight;
			index = i;
		}

		//texture1 = texture(ASCIIBrightness,vec2(0.0,index*1/FontSize));
		//texture2 = texture(ASCIIBrightness,vec2(0.0,i*1/FontSize));
		//if( abs(texture1.x-brightness) > abs(texture2.x-brightness) ) {
		//	index = i;
		//}
	}
	
	if(texture(ASCII,vec2(txtCoordX,index*1/FontSize+txtCoordY)).x==1.0) {
		Draw=true;
	}
	
	if(Draw)
		outColor = texture(frameBuffer, Texcoord);
	else
	    outColor = vec4(0.0,0.0,0.0,0.0);
}