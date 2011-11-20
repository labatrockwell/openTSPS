/** @namespace LAB.three */
LAB.three = LAB.three || {};

/** 
	@constructor 
	@extends THREE.Geometry
*/

LAB.three.Geometry = function () {
   THREE.Geometry.call( this );
   
   this.vertexEdges = [];
   this.vertexFaces = [];
   this.vertexNormals = [];
   
   this.faceEdges = [];
};

LAB.three.Geometry.prototype = new THREE.Geometry();
LAB.three.Geometry.prototype.constructor = LAB.three.Geometry;
LAB.three.Geometry.prototype.supr = THREE.Geometry.prototype;

/**
 @function
 @public
 */

LAB.three.Geometry.prototype.loadModel = function( modelLocation ){
   loader = new THREE.JSONLoader( true );
   callback = function( geometry ) {  };
   loader.load( { model: modelLocation });
}

LAB.three.Geometry.prototype.loadLabModel = function( model ){
   //vertex positions
   if( model.positions.length > 0 ){
      this.vertices = [];
      for(var i=0; i<model.positions.length; i++){
         this.vertices[i] = new THREE.Vertex( new THREE.Vector3(model.positions[i][0],
                                                                model.positions[i][1],
                                                                model.positions[i][2]));
      }
   }
   
   //vertex normals. these aren't loading to the three mesh yet. need to look into how three gets normals to the shader. I think its face->Vertex->Normals meaning seperate normals for each face vertex. not shared between faces
   if( model.normals.length ){// we should switch the exporter to write vertexNormals & faceNormals
      for(var i=0; i<model.normals.length; i++){
         this.vertexNormals[i] = new THREE.Vector3(model.normals[i][0],
                                                   model.normals[i][1],
                                                   model.normals[i][2]);
      }
   }
   
   
   for(var i=0; i<model.indices.length; i++ ){
      var fi = model.indices[i];
      if(fi.length == 3 ){
         this.faces.push( new THREE.Face3(fi[0],fi[1],fi[2] ) );
         
         if( model.texCoords ){
            this.faceVertexUvs[0][i] = [new THREE.UV( model.texCoords[ fi[0] ][0], model.texCoords[ fi[0] ][1]),
                                        new THREE.UV( model.texCoords[ fi[1] ][0], model.texCoords[ fi[1] ][1]),
                                        new THREE.UV( model.texCoords[ fi[2] ][0], model.texCoords[ fi[2] ][1])];
         }
      }
      else if( fi.length == 4){
         this.faces.push( new THREE.Face4(fi[0],fi[1],fi[2],fi[3] ) );
         
         if( model.texCoords ){
            this.faceVertexUvs[0][i] = [new THREE.UV( model.texCoords[ fi[0] ][0], model.texCoords[ fi[0] ][1]),
                                        new THREE.UV( model.texCoords[ fi[1] ][0], model.texCoords[ fi[1] ][1]),
                                        new THREE.UV( model.texCoords[ fi[2] ][0], model.texCoords[ fi[2] ][1]),
                                        new THREE.UV( model.texCoords[ fi[3] ][0], model.texCoords[ fi[3] ][1])];
         }
      }
   }
};

/**
 @function
 @public
 */
LAB.three.Geometry.prototype.addVertex = function( x,y,z){
   
   this.vertices.push( new THREE.Vertex( new THREE.Vector3(x,y,z)));
   return this.vertices.length - 1;
};

/**
	@function
	@public
*/
LAB.three.Geometry.prototype.addFace = function( i0, i1, i2 ){
   this.faces.push( new THREE.Face3( i0, i1, i2 ) );
   return this.faces.length - 1;
};
/**
	@function
	@public
*/
LAB.three.Geometry.prototype.update = function(){
   this.__dirtyVertices = true;
};
/**
	@function
	@public
*/
LAB.three.Geometry.prototype.calculateNormals = function( _smooth ){
   this.computeFaceNormals();
   var smooth = _smooth || true; // not reallly working yet. it seems that smoothing is handeled by the shader mesh setup
   if( smooth )   this.computeVertexNormals();
};


LAB.three.Geometry.prototype.getPointOnFace = function( faceIndex, u, v ){
   
   var f = this.faces[Math.min( faceIndex, this.faces.length - 1)];
   var fPos = new THREE.Vector3();
   
   if( f instanceof THREE.Face3){  
      /*
      http://en.wikipedia.org/wiki/Barycentric_coordinate_system_(mathematics)
      http://www.exaflop.org/docs/cgafaq/cga6.html 
      If u+v>1, replace a by 1-u, v by 1-v. Let w=1-u-v. Then a*u + b*v + c*w is uniformly distributed 
      in triangle ABC: the reflection step u=1-u; v=1-v gives a point (u,v) uniformly distributed in 
      the triangle (0,0)(1,0)(0,1), which is then mapped affinely to ABC. Now you have barycentric 
      coordinates a,b,c. Compute your point P = a*u + b*v + c*w.
       */
      
      if(u+v > 1){
         u = 1 - u;
         v = 1 - v;
      }
      var w = 1-u-v;
      var a = this.vertices[f.a].position;
      var b = this.vertices[f.b].position;
      var c = this.vertices[f.c].position;
      fPos.set(a.x*u + b.x*v + c.x * w,
               a.y*u + b.y*v + c.y * w,
               a.z*u + b.z*v + c.z * w );
      
   }
   else if(f instanceof THREE.Face4){
      //untested
      var mu = 1.0 - u;
      var mv = 1.0 - v;
      var a = this.vertices[f.a].position;
      var b = this.vertices[f.b].position;
      var c = this.vertices[f.c].position;
      var d = this.vertices[f.d].position;
      fPos.set((a.x*mu + b.x*u)*mv + (d.x*mu + c.x*u)*v,
               (a.y*mu + b.y*u)*mv + (d.y*mu + c.y*u)*v,
               (a.z*mu + b.z*u)*mv + (d.z*mu + c.z*u)*v);
   }
   
   return fPos;
};

LAB.three.Geometry.prototype.getSmoothedNormalOnFace = function( faceIndex, u, v ){
   var f = this.faces[Math.min( faceIndex, this.faces.length - 1)];
   var fNorm = new THREE.Vector3();
   
   if( f instanceof THREE.Face3){  
      
      if(u+v > 1){
         u = 1 - u;
         v = 1 - v;
      }
      var w = 1-u-v;
      var a = this.getVertexNormal( f.a );
      var b = this.getVertexNormal( f.b );
      var c = this.getVertexNormal( f.c );
      fNorm.set(a.x*u + b.x*v + c.x * w,
                a.y*u + b.y*v + c.y * w,
                a.z*u + b.z*v + c.z * w );
      
   }
   else if(f instanceof THREE.Face4){
      //untested
      var mu = 1.0 - u;
      var mv = 1.0 - v;
      var a = this.getVertexNormal( f.a );
      var b = this.getVertexNormal( f.b );
      var c = this.getVertexNormal( f.c );
      var d = this.getVertexNormal( f.d );
      fNorm.set((a.x*mu + b.x*u)*mv + (d.x*mu + c.x*u)*v,
                (a.y*mu + b.y*u)*mv + (d.y*mu + c.y*u)*v,
                (a.z*mu + b.z*u)*mv + (d.z*mu + c.z*u)*v);
   }
   
   return fNorm;
};

/*
 face
 
 */


LAB.three.Geometry.prototype.findEdges = function(){
   
   //create edges 
   for(var i=0; i<this.faces.length; i++){
      f = this.faces[i];
      if ( f instanceof THREE.Face3 ){
         var e = new THREE.Edge(this.vertices[f.a], this.vertices[f.b], f.a, f.b); 
         e.faces.push( f );
         e.faceIndices.push( i );
         this.edges.push( e );
         
         e = new THREE.Edge(this.vertices[f.b], this.vertices[f.c], f.b, f.c ); 
         e.faces.push( f );
         e.faceIndices.push( i );
         this.edges.push( e );
         
         e = new THREE.Edge(this.vertices[f.c], this.vertices[f.a], f.c, f.a); 
         e.faces.push( f );
         e.faceIndices.push( i );
         this.edges.push( e );
      }
      else if (f instanceof THREE.Face4 ){
         var e = new THREE.Edge(this.vertices[f.a], this.vertices[f.b], f.a, f.b); 
         e.faces.push( f );
         e.faceIndices.push( i );
         this.edges.push( e );
         
         e = new THREE.Edge(this.vertices[f.b], this.vertices[f.c], f.b, f.c ); 
         e.faces.push( f );
         e.faceIndices.push( i );
         this.edges.push( e );
         
         e = new THREE.Edge(this.vertices[f.c], this.vertices[f.d], f.c, f.d); 
         e.faces.push( f );
         e.faceIndices.push( i );
         this.edges.push( e );
         
         e = new THREE.Edge(this.vertices[f.d], this.vertices[f.a], f.d, f.a); 
         e.faces.push( f );
         e.faceIndices.push( i );
         this.edges.push( e );
      }
   }
   
   //merge duplicate edges
   var fi, fr;//face index and face reference
   function mergeEdges( _e1, _e2 ){
      for(var i=_e2.faces.length-1; i >= 0; i--){
         fr = _e2.faces.pop();
         fi = _e2.faceIndices.pop();
         
         for(var j=0; j<_e1.faces.length;j++){
            if( fi != _e1.faceIndices[j]){
               _e1.faces.push( fr );
               _e1.faceIndices.push( fi );
            }
         }
      }
   }
   
   var e1, e2;
   for(var i=0; i<this.edges.length; i++){
      e1 = this.edges[i];
      for(var j=i+1; j<this.edges.length; j++){
         e2 = this.edges[j];
         if( e1.vertexIndices[0] == e2.vertexIndices[0] && e1.vertexIndices[1] == e2.vertexIndices[1]){
            mergeEdges( e1, e2 );
         }
         else if(e1.vertexIndices[1] == e2.vertexIndices[0] && e1.vertexIndices[0] == e2.vertexIndices[1]){
            mergeEdges( e1, e2 );
         }
      }
   }
   
   for(var i=this.edges.length-1; i>=0; i--){
      if(this.edges[i].faces.length == 0){
         this.edges.splice( i, 1 );
      }
   }
//   LAB.log( this.edges.length );
//   LAB.log( this.edges );
};

LAB.three.Geometry.prototype.findVertexEdges = function(){
   if(this.edges.length == 0){
      this.findEdges();
   }
   
   //create or empty vertexEdges
   for(var i=0; i<this.vertices.length; i++){
      this.vertexEdges[i] = [];
   }
   
   //add edges to vertexEdges
   for(var i=0; i<this.edges.length; i++){
      //[ edgeIndex, edgeReference ]
      this.vertexEdges[ this.edges[i].vertexIndices[0] ].push( [ i, this.edges[i] ]);
      this.vertexEdges[ this.edges[i].vertexIndices[1] ].push( [ i, this.edges[i] ]);
   }
   //   LAB.log( this.vertexEdges.length );
   //   LAB.log( this.vertexEdges );
};

LAB.three.Geometry.prototype.findVertexFaces = function(){

   //create or empty vertexEdges
   for(var i=0; i<this.vertices.length; i++){
      this.vertexFaces[i] = [];
   }
   
   //add faces to vertexFaces
   var f;
   for(var i=0; i<this.faces.length; i++){
      f = this.faces[i];
      if(f instanceof THREE.Face3 ){
         this.vertexFaces[ f.a ].push( [ i, f ] );
         this.vertexFaces[ f.b ].push( [ i, f ] );
         this.vertexFaces[ f.c ].push( [ i, f ] );
      }
      else if(f instanceof THREE.Face4 ){
         this.vertexFaces[ f.a ].push( [ i, f ] );
         this.vertexFaces[ f.b ].push( [ i, f ] );
         this.vertexFaces[ f.c ].push( [ i, f ] );
         this.vertexFaces[ f.d ].push( [ i, f ] );
      }
   }
//   LAB.log( this.vertexFaces.length );
//   LAB.log( this.vertexFaces );
};


LAB.three.Geometry.prototype.findFaceEdges = function(){
   if(this.edges.length == 0){
      this.findEdges();
   }
   
   //create or empty faceEdges // is this the best/right way to do this?
   for(var i=0; i<this.faces.length; i++){
      this.faceEdges[i] = [];
   }
   
   //add edges to vertexEdges
   for(var i=0; i<this.edges.length; i++){
      
      for(var j=0; j<this.edges[i].faceIndices.length; j++) // non-manifold geometry can have n faces per edge
      {
         this.faceEdges[ this.edges[i].faceIndices[j] ].push( [ i, this.edges[i] ]);//[ edgeIndex, edgeReference ]
      }
   }
//   LAB.log( this.faceEdges.length );
//   LAB.log( this.faceEdges );
};


LAB.three.Geometry.prototype.getFaceNormal = function( faceIndex ){
   return this.faces[ faceIndex ].normal;
};
LAB.three.Geometry.prototype.getVertexNormal = function( vertexIndex ){
   //not as easy as it sounds in three o_O
   //for now we'll just calculate it using vertexFaces and store them in this.vertexNormals...
   if( !this.vertexNormals[vertexIndex] ){
      this.vertexNormals[vertexIndex] = new THREE.Vector3();
      this.calcVertexNormal( vertexIndex );
   }
   
   return this.vertexNormals[vertexIndex]
};

LAB.three.Geometry.prototype.calcVertexNormal = function( vertexIndex ){
   //not as easy as it sounds in three o_O
   //for now we'll calculate it using vertexFaces and store them in this.vertexNormals...
   if( !this.vertexNormals[vertexIndex] ){
      this.vertexNormals[vertexIndex] = new THREE.Vector3();
   }
   
   var vf = this.vertexFaces[ vertexIndex ];
   for(var i=0; i<vf.length; i++){
      this.vertexNormals[vertexIndex].addSelf( vf[i][1].normal );
   }
   
   return this.vertexNormals[vertexIndex].normalize();
};


LAB.three.Geometry.prototype.calculateVertexNormals = function(){
   for(var i=0; i<this.vertices.length; i++){
      this.calcVertexNormal( i );
   }
};




LAB.three.Geometry.prototype.getVertexLocalPosition = function( vertexIndex ){
   return this.vertices[vertexIndex].position;
};

