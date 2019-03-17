var vShader =
	'uniform mat4 uProjectionMatrix;'+
	'uniform mat4 uViewMatrix;'+
	'uniform mat4 uModelMatrix;'+
	'uniform bool uIsSpecular;'+
	''+
	'uniform vec3 uAmbientColor;' +
    'uniform vec3 uDiffuseColor;' +
	'' +
	'uniform vec3 uLightDir;'+
	''+
	'attribute vec3 aXYZ;'+
	'attribute vec3 aNormal;' +
	'attribute vec3 aColor;' +
	''+
	'varying vec3 vXYZ;' +
	'varying vec3 vPos;' +
	'varying vec3 vColor;' +
	'varying vec3 vNormal;'+
	''+
	'void main ()'+
	'{'+
	'	mat4 mvMatrix = uViewMatrix * uModelMatrix;' +
	'	vec4 pos = mvMatrix * vec4(aXYZ,1.0);'+
	'	gl_Position = uProjectionMatrix * pos;' +
	'' +
    '	vColor = uAmbientColor*aColor;'+
    ''+
    '	vec3 light = normalize(-uLightDir);'+
    '	vec3 normal = vec3(normalize(mvMatrix*vec4(aNormal,0)));'+
    '	vColor += aColor*uDiffuseColor*max(dot(normal,light),0.0);'+
	'	vXYZ = aXYZ;' +
	'	vNormal = normal;' +
	'	vPos = pos.xyz/pos.w;'+
	'}';
	
var fShader =
	'precision mediump float;' +
	'' +
	'uniform highp vec3 uLightDir;' +
	'uniform vec3 uSpecularColor;' +
	'uniform float uShininess;' +
	'uniform bool uUseSpecular;' +
	'uniform bool uUseBlur;'+
	'uniform float uBlurRate;'+
	''+
	'uniform samplerCube uTexUnit;'+
	'varying vec3 vXYZ;' +
	'varying vec3 vPos;' +
	'varying vec3 vNormal;' +
	'varying vec3 vColor;' +
	''+
	'void main( )'+
	'{' +
	'	vec3 light = normalize(-uLightDir);' +
	'	vec3 reflectedLight = normalize(reflect(light, normalize(vNormal)));' +
	'	vec3 viewDir = normalize(vPos);' +
	'' +
	'	vec3 color = vec3(0.0,0.0,0.0);' +
	'	if (uUseSpecular)' +
	'	{' +
    '	float cosa = max(dot(reflectedLight, viewDir), 0.0);' +
    '	color = max(vColor*uSpecularColor*pow(cosa, uShininess), 0.0);' +
	'	}' +
	'' +
	'' +
	'	vec3 finalColor = (color + textureCube(uTexUnit,vXYZ).stp*vColor);' +
	'' +
	'	if (uUseBlur)' +
	'	{' +
	'		finalColor /= 10.0;' +
	'		finalColor += 0.1*(color + textureCube(uTexUnit,vec3(vXYZ.x + uBlurRate, vXYZ.y, vXYZ.z)).stp*vColor);' +
    '		finalColor += 0.1*(color + textureCube(uTexUnit,vec3(vXYZ.x - uBlurRate, vXYZ.y, vXYZ.z)).stp*vColor);' +
    '		finalColor += 0.1*(color + textureCube(uTexUnit,vec3(vXYZ.x, vXYZ.y + uBlurRate, vXYZ.z)).stp*vColor);' +
    '		finalColor += 0.1*(color + textureCube(uTexUnit,vec3(vXYZ.x, vXYZ.y - uBlurRate, vXYZ.z)).stp*vColor);' +
    '		finalColor += 0.1*(color + textureCube(uTexUnit,vec3(vXYZ.x + 2.0*uBlurRate, vXYZ.y, vXYZ.z)).stp*vColor);' +
    '		finalColor += 0.1*(color + textureCube(uTexUnit,vec3(vXYZ.x - 2.0*uBlurRate, vXYZ.y, vXYZ.z)).stp*vColor);' +
    '		finalColor += 0.1*(color + textureCube(uTexUnit,vec3(vXYZ.x, vXYZ.y + 2.0*uBlurRate, vXYZ.z)).stp*vColor);' +
    '		finalColor += 0.1*(color + textureCube(uTexUnit,vec3(vXYZ.x, vXYZ.y - 2.0*uBlurRate, vXYZ.z)).stp*vColor);' +
	'	}' +
	'' +
	'	'+
	'	gl_FragColor = vec4(finalColor,1.0);'+
	'}';
