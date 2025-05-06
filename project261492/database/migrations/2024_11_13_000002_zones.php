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
        Schema::create('zones', function (Blueprint $table) {
            $table->id()->primary();
            $table->text('description')->nullable();
            $table->string('location', 64)->nullable();
        });

        Schema::create('access_zones', function (Blueprint $table) {
            $table->foreignId('entity')->references('id')->on('entities');
            $table->foreignId('access_zone')->references('id')->on('zones');
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('access_zones');
        Schema::dropIfExists('zones');
    }
};
