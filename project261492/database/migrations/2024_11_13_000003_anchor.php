<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     */
    public function up(): void
    {
        Schema::create('anchor_locations', function (Blueprint $table) {
            $table->foreignId('anchor')->primary()->references('id')->on('entities');
            $table->float('location_X', precision: 24);
            $table->float('location_Y', precision: 24);
            $table->float('location_Z', precision: 24);
        });

        Schema::create('controller_have_anchors', function (Blueprint $table) {
            $table->foreignId('controller')->references('id')->on('entities');
            $table->foreignId('have_anchor')->references('id')->on('entities');
            $table->unsignedInteger('on_port');
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('controller_have_anchors');
        Schema::dropIfExists('anchor_locations');
    }
};
