#version 430 core
in vec2 Texcoord;

out vec4 outColor;

layout(rgba8,binding=0) uniform image2D frameBuffer;
uniform sampler2D ASCII;
uniform sampler2D ASCIIBrightness;
uniform sampler2D ASCIIHistogram;
uniform sampler2D screenHistogram;
uniform sampler2D srcTex;

const vec2 pixelSize = vec2(8.0,8.0);
const float screenWidth = 800.0;
const float screenHeight = 600.0;
const float FontX = 8.0;
const float FontY = 8.0;
const float FontSize = 256.0;

void main()
{
	float txtCoordX=mod(Texcoord.x*800,FontX)*1/FontX;
	float txtCoordY=mod(Texcoord.y*600,FontY)*1/(FontY*FontSize);

	vec2 xyClamped = vec2(
		Texcoord.x*800-mod(Texcoord.x*800,FontX),
		Texcoord.y*600-mod(Texcoord.y*600,FontY)
	);

	if( xyClamped.x < FontX )
		xyClamped.x = FontX;
	if( xyClamped.y < FontY )
		xyClamped.y = FontY;

	vec4 brightnessVec = vec4(0.0);
	/*for(int i=0;i<FontY;i++) {
		for(int j=0;j<FontX;j++) {
			brightnessVec += texture(frameBuffer,
				vec2( (xyClamped.x+j)/800, (xyClamped.y+i)/600 )
			);
		}
	}*/

	bool Draw = false;

	//brightnessVec=brightnessVec/(FontX*FontY);
	/*brightnessVec=texture(frameBuffer,
				vec2( (xyClamped.x)/800, (xyClamped.y)/600 )
			);
	float brightness = (brightnessVec.r + brightnessVec.g + brightnessVec.b)/3.0;

	float index = 0.0;
	float maxWeight = 0.0;
	float currWeight = 0.0;
	for(float i=0;i<FontSize;i++) {
		currWeight = 0.0;
		currWeight+=1.0 - abs(
			texture(ASCIIBrightness,vec2(0.0,i*1/FontSize)).x -
			brightness
		);
		if(maxWeight < currWeight) {
			maxWeight = currWeight;
			index = i;
		}
	}

	if( texture(ASCII,vec2(txtCoordX,index*1/FontSize+txtCoordY)).x==1.0 ) {
		Draw=true;
	}*/
	
	//if(Draw)
	//	outColor = texture(frameBuffer, Texcoord);
	//else
	//    outColor = vec4(0.0,0.0,0.0,0.0);

	ivec2 coord = ivec2(int(Texcoord.x*800),int(Texcoord.y*600));

	//imageStore(frameBuffer, coord, vec4(1.0, 0.0, 0.0, 0.0));
	outColor = imageLoad(frameBuffer, coord);
}