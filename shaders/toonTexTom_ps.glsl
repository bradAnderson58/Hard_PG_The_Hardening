varying vec3 vNormal;
varying vec3 vVertex;

uniform sampler2D tex;

void main(void) 
{
	vec4 materialColor = gl_FrontMaterial.diffuse;
	vec4 silhouetteColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 texture = texture2D(tex,gl_TexCoord[0].st);

	vec3 eyePos = normalize(-vVertex);
	vec3 lightPos = gl_LightSource[0].position.xyz;
	vec3 Normal = normalize(vNormal);
	
	vec3 LightDir = normalize(lightPos - vVertex);
	vec3 EyeDir = normalize(eyePos - vVertex); 
	vec3 EyeLight = normalize(LightDir + EyeDir);

	vec4 ambient = vec4(0.0);
	vec4 diffuse = texture * materialColor;
	vec4 specular = vec4(0.5);
	
	vec3 reflection = reflect(LightDir, vNormal);

	// draw silhouette
	float sil = max(dot(Normal,EyeDir), 0.0);
	if( sil < 0.15 )
	   gl_FragColor = silhouetteColor;
	
	// inside silhouette
	else
	{
		// how much we dim or glint by
		float kD = max(dot(Normal, LightDir), 0.0);	// diffuse coef
		float kS = pow(max(dot(Normal, EyeLight), 0.0), 16.0); // spec coef
		
		if (kD < 0.1)
			kD = 0.3;
		else
			kD = 1.0;

		if (kS < 0.3)
			kS = 0.1;
		else if (kS < 0.5)
			kS = 0.3;
		else
			kS = 1.0;

		diffuse *= kD;
		specular *= kS;
		
		gl_FragColor = ambient + diffuse + specular;
	}
}